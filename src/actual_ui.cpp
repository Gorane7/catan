// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <iostream>
#include <cmath>

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


GLuint generateBuffer(GLfloat bufferData[], int size) {
  GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, size, bufferData, GL_STATIC_DRAW);
  return buffer;
}

void bindBuffer(GLuint buffer, int id, int size) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glVertexAttribPointer(
    id,                               // attribute. No particular reason for 1, but must match the layout in the shader.
    size,                             // size
    GL_FLOAT,                         // type
    GL_FALSE,                         // normalized?
    0,                                // stride
    (void*)0                          // array buffer offset
  );
}

void clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

ActualUI::ActualUI(int playerAmount) {
  srand (static_cast <unsigned> (time(0)));
  game = createGame(playerAmount);
  // Calculate tile vertices from definition
  for (int i = 0; i < 6 * 3; i++) {
    tile_deltas[i * 3 + 0] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 0];
    tile_deltas[i * 3 + 1] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 1];
    tile_deltas[i * 3 + 2] = tile_delta_vertice_coordinates[tile_delta_vertices[i] * 3 + 2];
  }
  // Calculate road vertices from definition
  for (int i = 0; i < 12 * 3; i++) {
    road_deltas[i * 3 + 0] = road_delta_vertice_coordinates[road_delta_vertices[i] * 3 + 0];
    road_deltas[i * 3 + 1] = road_delta_vertice_coordinates[road_delta_vertices[i] * 3 + 1];
    road_deltas[i * 3 + 2] = road_delta_vertice_coordinates[road_delta_vertices[i] * 3 + 2];
  }
}

void ActualUI::run() {
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
	glm::mat4 Projection = glm::perspective(glm::radians(55.0f), 4.0f / 3.0f, 0.1f, 100.0f);
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

  GLfloat city_vertex_buffer_data[MAX_PLAYERS * float_per_city];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_city; j++) {
      city_vertex_buffer_data[i * float_per_city + j] = city_deltas[j];
    }
  }


  GLfloat city_colour_buffer_data[MAX_PLAYERS * float_per_city];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_city; j++) {
      city_colour_buffer_data[i * float_per_city + j] = player_colours[i * 3 + j % 3];
    }
  }

  GLfloat road_vertex_buffer_data[MAX_PLAYERS * float_per_road];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_road; j++) {
      road_vertex_buffer_data[i * float_per_road + j] = road_deltas[j];
    }
  }


  GLfloat road_colour_buffer_data[MAX_PLAYERS * float_per_road];
  for (int i = 0; i < MAX_PLAYERS; i++) {
    for (int j = 0; j < float_per_road; j++) {
      road_colour_buffer_data[i * float_per_road + j] = player_colours[i * 3 + j % 3];
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

  GLuint tileVertexBuffer = generateBuffer(tile_vertex_buffer_data, sizeof(tile_vertex_buffer_data));
  GLuint tileColourBuffer = generateBuffer(tile_colour_buffer_data, sizeof(tile_colour_buffer_data));
  GLuint numberVertexBuffer = generateBuffer(number_vertex_buffer_data, sizeof(number_vertex_buffer_data));
  GLuint numberColourBuffer = generateBuffer(number_colour_buffer_data, sizeof(number_colour_buffer_data));
  GLuint villageVertexBuffer = generateBuffer(village_vertex_buffer_data, sizeof(village_vertex_buffer_data));
  GLuint villageColourBuffer = generateBuffer(village_colour_buffer_data, sizeof(village_colour_buffer_data));
  GLuint cityVertexBuffer = generateBuffer(city_vertex_buffer_data, sizeof(city_vertex_buffer_data));
  GLuint cityColourBuffer = generateBuffer(city_colour_buffer_data, sizeof(city_colour_buffer_data));
  GLuint roadVertexBuffer = generateBuffer(road_vertex_buffer_data, sizeof(road_vertex_buffer_data));
  GLuint roadColourBuffer = generateBuffer(road_colour_buffer_data, sizeof(road_colour_buffer_data));

  glfwSwapInterval(0);


  int lastTime = time(0);
  int framesSinceLast = 0;
  int simsEnded = 0;
  int startTime = time(0);
  std::cout << "\n";
	do{
    int thisTime = time(0);
    if (game.winner == -1 /*&& thisTime - lastTime*/) {
      askAction(game);
      displayState(game);
      lastTime = thisTime;
      std::cout << framesSinceLast << " frames per second\n";
      framesSinceLast = 0;
    }
    if (game.winner != -1 && simsEnded == 0) {
      simsEnded++;
      std::cout << "Simulating game took " << time(0) - startTime << " seconds\n";
      std::cout << "Game ended on turn " << game.turnAmount << "\n";
      std::cout << "Simulating one turn took " << 1000 * (time(0) - startTime) / (float) game.turnAmount << " ms\n";
      std::cout << "Winner was player " << game.winner << "\n";
    }
    framesSinceLast++;

    clearScreen();


		glUseProgram(programID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    bindBuffer(tileVertexBuffer, 0, 3);
    bindBuffer(tileColourBuffer, 1, 3);
    glDrawArrays(GL_TRIANGLES, 0, TILE_AMOUNT * vertex_per_tile); // 12*3 indices starting at 0 -> 12 triangles


		glUseProgram(UVprogramID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		glUniform1i(TextureID, 0);
    bindBuffer(numberVertexBuffer, 0, 3);
    bindBuffer(numberColourBuffer, 1, 2);
    glDrawArrays(GL_TRIANGLES, 0, NUMBER_AMOUNT * vertex_per_number);


    glUseProgram(programID);
    bindBuffer(villageVertexBuffer, 0, 3);
    bindBuffer(villageColourBuffer, 1, 3);
    for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
      if (game.board.villages[i] == -1) {
        continue;
      }

      int tileX = villageArrToTileX(i);
      int tileY = villageArrToTileY(i);
      bool isUpper = villageArrToUpperBool(i);

      glm::mat4 modelVillage = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.732f * tileX + 0.866f * tileY, 1.5f * tileY + (isUpper ? 1.0f : -1.0f), 0.0f, 1.0f
      );
      glm::mat4 thisVillage = Projection * View * modelVillage; // Remember, matrix multiplication is the other way around

      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &thisVillage[0][0]);
      glDrawArrays(GL_TRIANGLES, game.board.villages[i] * vertex_per_village, vertex_per_village); // 12*3 indices starting at 0 -> 12 triangles
    }


    glUseProgram(programID);
    bindBuffer(cityVertexBuffer, 0, 3);
    bindBuffer(cityColourBuffer, 1, 3);
    for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
      if (game.board.cities[i] == -1) {
        continue;
      }

      int tileX = villageArrToTileX(i);
      int tileY = villageArrToTileY(i);
      bool isUpper = villageArrToUpperBool(i);

      glm::mat4 modelVillage = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        1.732f * tileX + 0.866f * tileY, 1.5f * tileY + (isUpper ? 1.0f : -1.0f), 0.0f, 1.0f
      );
      glm::mat4 thisVillage = Projection * View * modelVillage; // Remember, matrix multiplication is the other way around

      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &thisVillage[0][0]);
      glDrawArrays(GL_TRIANGLES, game.board.cities[i] * vertex_per_city, vertex_per_city); // 12*3 indices starting at 0 -> 12 triangles
    }


    bindBuffer(roadVertexBuffer, 0, 3);
    bindBuffer(roadColourBuffer, 1, 3);
    glUseProgram(programID);
    for (int i = 0; i < ROAD_ARRAY_LENGTH; i++) {
      if (game.board.roads[i] == -1) {
        continue;
      }

      int villageA = i / VILLAGE_ARRAY_LENGTH;
      int villageB = i % VILLAGE_ARRAY_LENGTH;

      int tileXA = villageArrToTileX(villageA);
      int tileYA = villageArrToTileY(villageA);
      bool isUpperA = villageArrToUpperBool(villageA);
      int tileXB = villageArrToTileX(villageB);
      int tileYB = villageArrToTileY(villageB);
      bool isUpperB = villageArrToUpperBool(villageB);

      float dx = 0.866f * (tileXA - tileXB) + 0.433f * (tileYA - tileYB);
      float dy = 0.75f * (tileYA - tileYB) + (isUpperA ? 0.5f : -0.5f) - (isUpperB ? 0.5f : -0.5f);
      float angle = std::atan2(dy, dx);

      glm::mat4 modelRoad = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.866f * (tileXA + tileXB) + 0.433f * (tileYA + tileYB), 0.75f * (tileYA + tileYB) + (isUpperA ? 0.5f : -0.5f) + (isUpperB ? 0.5f : -0.5f), 0.0f, 1.0f
      );

      glm::vec3 myRotationAxis(0.0f, 0.0f, 1.0f);
      glm::mat4 roadRotation = glm::rotate(angle, myRotationAxis);

      glm::mat4 thisRoad = Projection * View * modelRoad * roadRotation; // Remember, matrix multiplication is the other way around

      glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &thisRoad[0][0]);
      glDrawArrays(GL_TRIANGLES, game.board.roads[i] * vertex_per_road, vertex_per_road); // 12*3 indices starting at 0 -> 12 triangles
    }


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &tileVertexBuffer);
	glDeleteBuffers(1, &tileColourBuffer);
  glDeleteBuffers(1, &numberColourBuffer);

  glDeleteBuffers(1, &numberVertexBuffer);
  glDeleteBuffers(1, &villageVertexBuffer);
  glDeleteBuffers(1, &villageColourBuffer);

	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return;
}
