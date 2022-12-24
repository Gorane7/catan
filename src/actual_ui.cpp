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

  GLfloat village_vertex_buffer_data[VILLAGE_LOCATION_AMOUNT * float_per_village];
  for (int i = 0; i < VILLAGE_LOCATION_AMOUNT; i++) {
    for (int j = 0; j < float_per_village; j++) {
      village_vertex_buffer_data[i * float_per_village + j] = village_centers[i * 3 + j % 3] + village_deltas[j];
    }
  }


  GLfloat village_colour_buffer_data[VILLAGE_LOCATION_AMOUNT * float_per_village];
  for (int i = 0; i < VILLAGE_LOCATION_AMOUNT; i++) {
    for (int j = 0; j < float_per_village; j++) {
      village_colour_buffer_data[i * float_per_village + j] = 0.1f;
    }
  }


  // tile amount * triangle per tile * vertex per triangle * float per vertex
  GLfloat g_vertex_buffer_data[TILE_AMOUNT * float_per_tile];
  for (int i = 0; i < TILE_AMOUNT; i++) {
    for (int j = 0; j < float_per_tile; j++) {
      g_vertex_buffer_data[i * float_per_tile + j] = tile_centers[i * 3 + j % 3] + tile_deltas[j];
    }
  }

  // tile amount * triangle per tile * vertex per triangle * float per vertex
	GLfloat g_color_buffer_data[TILE_AMOUNT * float_per_tile];
	for (int v = 0; v < TILE_AMOUNT ; v++){
    float c1 = tile_colours[game.board.tiles[v] * 3 + 0];
    float c2 = tile_colours[game.board.tiles[v] * 3 + 1];
    float c3 = tile_colours[game.board.tiles[v] * 3 + 2];
    for (int b = 0; b < vertex_per_tile; b++) {
      g_color_buffer_data[float_per_tile*v+b*3+0] = c1;
      g_color_buffer_data[float_per_tile*v+b*3+1] = c2;
      g_color_buffer_data[float_per_tile*v+b*3+2] = c3;
    }
	}
  /*for (int i = 0; i < TILE_AMOUNT * float_per_number; i++) {
    g_color_buffer_data[TILE_AMOUNT * float_per_tile + i] = 0.9f;
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

  GLfloat g_uv_vertex_buffer_data[NUMBER_AMOUNT * float_per_number];
  int desertsFound = 0;
  for (int i = 0; i < TILE_AMOUNT; i++) {
    if (game.board.tiles[i] == DESERT) {
      desertsFound++;
      continue;
    }
    for (int j = 0; j < float_per_number; j++) {
      g_uv_vertex_buffer_data[(i - desertsFound) * float_per_number + j] = tile_centers[i * 3 + j % 3] + square_deltas[j];
      if (j % 3 == 2) {
        g_uv_vertex_buffer_data[(i - desertsFound) * float_per_number + j] += square_height;
      }
    }
  }

	GLfloat g_uv_buffer_data[NUMBER_AMOUNT * 6 * 2];
	for (int i = 0; i < NUMBER_AMOUNT; i++) {
    int number = game.board.numbers[i];
    float basex = number_bases[number * 2 + 0];
    float basey = number_bases[number * 2 + 1];
		g_uv_buffer_data[i * 12 + 0] = basex + small;
		g_uv_buffer_data[i * 12 + 1] = basey + large;

		g_uv_buffer_data[i * 12 + 2] = basex + large;
		g_uv_buffer_data[i * 12 + 3] = basey + large;

		g_uv_buffer_data[i * 12 + 4] = basex + small;
		g_uv_buffer_data[i * 12 + 5] = basey + small;

    g_uv_buffer_data[i * 12 + 6] = basex + large;
		g_uv_buffer_data[i * 12 + 7] = basey + small;

		g_uv_buffer_data[i * 12 + 8] = basex + large;
		g_uv_buffer_data[i * 12 + 9] = basey + large;

		g_uv_buffer_data[i * 12 + 10] = basex + small;
		g_uv_buffer_data[i * 12 + 11] = basey + small;
	}

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

  GLuint uvvertexbuffer;
	glGenBuffers(1, &uvvertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvvertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_vertex_buffer_data), g_uv_vertex_buffer_data, GL_STATIC_DRAW);

  GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  GLuint villagevertexbuffer;
	glGenBuffers(1, &villagevertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, villagevertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(village_vertex_buffer_data), village_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint villagecolorbuffer;
	glGenBuffers(1, &villagecolorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, villagecolorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(village_colour_buffer_data), village_colour_buffer_data, GL_STATIC_DRAW);


  int lastTime = time(0);
  std::cout << "\n";
	do{
    int thisTime = time(0);
    if (thisTime - lastTime) {
      askAction(game);
      lastTime = thisTime;

    }
		// glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    // Vertices
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
    // Colours
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
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
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
    glDrawArrays(GL_TRIANGLES, 0, NUMBER_AMOUNT * vertex_per_number);


    glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
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
		glBindBuffer(GL_ARRAY_BUFFER, villagecolorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
		glDrawArrays(GL_TRIANGLES, 0, VILLAGE_LOCATION_AMOUNT * float_per_village); // 12*3 indices starting at 0 -> 12 triangles


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
  glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return;
}
