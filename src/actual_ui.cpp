// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

#include "shader.hpp"
#include "game_board.hpp"
#include "game.hpp"
#include "actual_ui.hpp"
#include "texture.hpp"
#include "randomAI.hpp"
#include "terminal_ui.hpp"

void run() {
  int playerAmount = 4;
  Game game = createGame(playerAmount);

  float tile_deltas[6 * 3 * 3];
  for (int i = 0; i < 6 * 3; i++) {
    tile_deltas[i * 3 + 0] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 0];
    tile_deltas[i * 3 + 1] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 1];
    tile_deltas[i * 3 + 2] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 2];
  }

  srand (static_cast <unsigned> (time(0)));
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 04 - Colored Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		getchar();
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders(
    "TransformVertexShader.vertexshader",
    "ColorFragmentShader.fragmentshader"
  );

  GLuint UVprogramID = LoadShaders(
    "UVVertexShader.vertexshader",
    "UVFragmentShader.fragmentshader"
  );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

  GLuint Texture = loadBMP_custom("numbers.bmp");
  GLuint TextureID  = glGetUniformLocation(UVprogramID, "myTextureSampler");

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(50.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View  = glm::lookAt(
	  glm::vec3(-0.5,-2,10), // Camera is at (4,3,-3), in World Space
		glm::vec3(0,-0.2,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

  GLfloat village_vertex_buffer_data[MAX_PLAYERS * float_per_village];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_village; j++) {
      village_vertex_buffer_data[i * float_per_village + j] = village_deltas[j];
    }
  }


  GLfloat village_colour_buffer_data[MAX_PLAYERS * float_per_village];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_village; j++) {
      village_colour_buffer_data[i * float_per_village + j] = player_colours[i * 3 + j % 3];
      //std::cout << village_colour_buffer_data[i * float_per_village + j] << "\n";
    }
  }


  // tile amount * triangle per tile * vertex per triangle * float per vertex
  GLfloat tile_vertex_buffer_data[TILE_AMOUNT * float_per_tile];
  int c = 0;
  for (int i = 0; i < MAP_WIDTH * MAP_WIDTH; i++) {
    if (!isValidTile(i)) {
      continue;
    }
    int x = i % MAP_WIDTH - MAP_RADIUS;
    int y = i / MAP_WIDTH - MAP_RADIUS;
    float d[3] = {1.732f * x + 0.866f * y, 1.5f * y, 0.0f};
    for (int j = 0; j < vertex_per_tile; j++) {
      for (int k = 0; k < 3; k++) {
        tile_vertex_buffer_data[c * float_per_tile + j * 3 + k] = d[k] + tile_deltas[j * 3 + k];
      }
    }
    c++;
  }

  // tile amount * triangle per tile * vertex per triangle * float per vertex
	GLfloat tile_colour_buffer_data[TILE_AMOUNT * float_per_tile];
	for (int v = 0; v < TILE_AMOUNT ; v++){
    float c1 = tile_colours[game.board.abstractTiles[v] * 3 + 0];
    float c2 = tile_colours[game.board.abstractTiles[v] * 3 + 1];
    float c3 = tile_colours[game.board.abstractTiles[v] * 3 + 2];
    for (int b = 0; b < vertex_per_tile; b++) {
      tile_colour_buffer_data[float_per_tile*v+b*3+0] = c1;
      tile_colour_buffer_data[float_per_tile*v+b*3+1] = c2;
      tile_colour_buffer_data[float_per_tile*v+b*3+2] = c3;
    }
	}
  /*for (int i = 0; i < TILE_AMOUNT * float_per_number; i++) {
    tile_colour_buffer_data[TILE_AMOUNT * float_per_tile + i] = 0.9f;
  }*/

  static const float small = 0.0f;
	static const float large = 0.25f;
  static const float number_bases[] = {
    0.5f, 0.25f, // 0
    0.5f, 0.25f, // 1
    0.0f, 0.75f, // 2
    0.25f, 0.75f, // 3
    0.5f, 0.75f, // 4
    0.75f, 0.75f, // 5
    0.0f, 0.5f, // 6
    0.5f, 0.25f, // 7
    0.25f, 0.5f, // 8
    0.5f, 0.5f, // 9
    0.75f, 0.5f, // 10
    0.0f, 0.25f, // 11
    0.25f, 0.25f, // 12
  };

  GLfloat number_vertex_buffer_data[NUMBER_AMOUNT * float_per_number];
  int desertsFound = 0;
  c = 0;
  for (int i = 0; i < MAP_WIDTH * MAP_WIDTH; i++) {
    if (!isValidTile(i)) {
      continue;
    }
    if (game.board.abstractTiles[c + desertsFound] == DESERT) {
      desertsFound++;
      continue;
    }
    int x = i % MAP_WIDTH - MAP_RADIUS;
    int y = i / MAP_WIDTH - MAP_RADIUS;
    float d[3] = {1.732f * x + 0.866f * y, 1.5f * y, 0.0f};
    for (int j = 0; j < vertex_per_number; j++) {
      for (int k = 0; k < 3; k++) {
        number_vertex_buffer_data[c * float_per_number + j * 3 + k] = d[k] + square_deltas[j * 3 + k];
      }
      number_vertex_buffer_data[c * float_per_number + j * 3 + 2] += square_height;
    }
    c++;
  }

	GLfloat number_colour_buffer_data[NUMBER_AMOUNT * 6 * 2];
	for (int i = 0; i < NUMBER_AMOUNT; i++) {
    int number = game.board.numbers[i];
    float basex = number_bases[number * 2 + 0];
    float basey = number_bases[number * 2 + 1];
		number_colour_buffer_data[i * 12 + 0] = basex + small;
		number_colour_buffer_data[i * 12 + 1] = basey + large;

		number_colour_buffer_data[i * 12 + 2] = basex + large;
		number_colour_buffer_data[i * 12 + 3] = basey + large;

		number_colour_buffer_data[i * 12 + 4] = basex + small;
		number_colour_buffer_data[i * 12 + 5] = basey + small;

    number_colour_buffer_data[i * 12 + 6] = basex + large;
		number_colour_buffer_data[i * 12 + 7] = basey + small;

		number_colour_buffer_data[i * 12 + 8] = basex + large;
		number_colour_buffer_data[i * 12 + 9] = basey + large;

		number_colour_buffer_data[i * 12 + 10] = basex + small;
		number_colour_buffer_data[i * 12 + 11] = basey + small;
	}

	GLuint tilevertexbuffer;
	glGenBuffers(1, &tilevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tilevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile_vertex_buffer_data), tile_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint tilecolourbuffer;
	glGenBuffers(1, &tilecolourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, tilecolourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tile_colour_buffer_data), tile_colour_buffer_data, GL_STATIC_DRAW);

  GLuint uvvertexbuffer;
	glGenBuffers(1, &uvvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(number_vertex_buffer_data), number_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint uvcolourbuffer;
	glGenBuffers(1, &uvcolourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvcolourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(number_colour_buffer_data), number_colour_buffer_data, GL_STATIC_DRAW);

  GLuint villagevertexbuffer;
	glGenBuffers(1, &villagevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, villagevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(village_vertex_buffer_data), village_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint villagecolourbuffer;
	glGenBuffers(1, &villagecolourbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, villagecolourbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(village_colour_buffer_data), village_colour_buffer_data, GL_STATIC_DRAW);

  glfwSwapInterval(0);


  int lastTime = time(0);
  int framesSinceLast = 0;
  std::cout << "\n";
	do{
    int thisTime = time(0);
    if (thisTime - lastTime) {
      askAction(game);
      lastTime = thisTime;
      std::cout << framesSinceLast << " frames per second\n";
      framesSinceLast = 0;
    }
    framesSinceLast++;

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // Vertices
		glBindBuffer(GL_ARRAY_BUFFER, tilevertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
    // Colours
		glBindBuffer(GL_ARRAY_BUFFER, tilecolourbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, TILE_AMOUNT * vertex_per_tile); // 12*3 indices starting at 0 -> 12 triangles


		glUseProgram(UVprogramID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // Additional stuff for textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);
    // Vertices
		glBindBuffer(GL_ARRAY_BUFFER, uvvertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
    // Colours
		glBindBuffer(GL_ARRAY_BUFFER, uvcolourbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
    glDrawArrays(GL_TRIANGLES, 0, NUMBER_AMOUNT * vertex_per_number);


    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, villagevertexbuffer);
    glVertexAttribPointer(
      0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void*)0            // array buffer offset
    );
    // Colours
    glBindBuffer(GL_ARRAY_BUFFER, villagecolourbuffer);
    glVertexAttribPointer(
      1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
      3,                                // size
      GL_FLOAT,                         // type
      GL_FALSE,                         // normalized?
      0,                                // stride
      (void*)0                          // array buffer offset
    );
    for (int i = 0; i < VILLAGE_LOCATION_AMOUNT; i++) {
      if (game.board.villages[i] == -1) {
        continue;
      }
      //glm::mat4 modelVillage = glm::translate(glm::mat4(), glm::vec3(village_centers[i * 3], village_centers[i * 3 + 1], village_centers[i * 3 + 2]));
      glm::mat4 modelVillage = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        village_centers[i * 3], village_centers[i * 3 + 1], village_centers[i * 3 + 2], 1.0f
      );
      glm::mat4 thisVillage = Projection * View * modelVillage; // Remember, matrix multiplication is the other way around
      glUseProgram(programID);
      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &thisVillage[0][0]);

  		glDrawArrays(GL_TRIANGLES, game.board.villages[i] * vertex_per_village, vertex_per_village); // 12*3 indices starting at 0 -> 12 triangles
    }


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &tilevertexbuffer);
	glDeleteBuffers(1, &tilecolourbuffer);
  glDeleteBuffers(1, &uvcolourbuffer);

  glDeleteBuffers(1, &uvvertexbuffer);
  glDeleteBuffers(1, &villagevertexbuffer);
  glDeleteBuffers(1, &villagecolourbuffer);

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return;
}
