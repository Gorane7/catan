#ifndef ACTUAL_UI_HPP
#define ACTUAL_UI_HPP

#include "game_board.hpp"
#include "game.hpp"

static const int vertex_per_tile = 18;
static const int float_per_tile = vertex_per_tile * 3;
static const int vertex_per_number = 6;
static const int float_per_number = vertex_per_number * 3;
static const float square_height = 0.01f;
static const int vertex_per_village = 12 * 3;
static const int float_per_village = vertex_per_village * 3;
static const int vertex_per_road = 12 * 3;
static const int float_per_road = vertex_per_road * 3;
static const float hex_edge_length = 1.0f;
static const float road_length_half = 0.3f;
static const float road_radius = 0.05f;

static const float road_delta_vertice_coordinates[8 * 3] = {
  -road_length_half, -road_radius, 0.0f,  // left front down
  -road_length_half, road_radius, 0.0f,  // left back down
  -road_length_half, -road_radius, 2 * road_radius,  // left front up
  -road_length_half, road_radius, 2 * road_radius,  // left back up
  road_length_half, -road_radius, 0.0f,  // right front down
  road_length_half, road_radius, 0.0f,  // right back down
  road_length_half, -road_radius, 2 * road_radius,  // right front up
  road_length_half, road_radius, 2 * road_radius,  // right back up
};

static const int road_delta_vertices[12 * 3] = {
  0, 2, 1,
  1, 3, 2,
  4, 6, 5,
  5, 7, 6,
  0, 4, 5,
  0, 1, 5,
  2, 6, 7,
  2, 3, 7,
  0, 4, 6,
  0, 2, 6,
  1, 5, 7,
  1, 3, 7
};

static const float tile_delta_vertice_coordinates[7 * 3] = {
  0.0f, 0.0f, 0.0f,     // center
  -0.866f, 0.5f, 0.0f,  // left top
  0.0f, 1.0f, 0.0f,     // top
  0.866f, 0.5f, 0.0f,  // right top
  0.866f, -0.5f, 0.0f,  // right bottom
  0.0f, -1.0f, 0.0f,     // bottom
  -0.866f, -0.5f, 0.0f  // left bottom
};

static const int tile_delta_vertices[6 * 3] = {
  0, 1, 2,
  0, 2, 3,
  0, 3, 4,
  0, 4, 5,
  0, 5, 6,
  0, 6, 1
};

static const float tile_colours[6 * 3] = {
  1.0f, 1.0f, 0.5f,
  0.1f, 0.5f, 0.1f,
  0.1f, 1.0f, 0.1f,
  0.9f, 0.9f, 0.1f,
  0.7f, 0.7f, 0.7f,
  0.5f, 0.2f, 0.2f
};

static const float square_radius = 0.2f;
static const float square_deltas[6 * 3] = {
  -square_radius, square_radius, 0.0f,
  square_radius, square_radius, 0.0f,
  -square_radius, -square_radius, 0.0f,
  square_radius, -square_radius, 0.0f,
  square_radius, square_radius, 0.0f,
  -square_radius, -square_radius, 0.0f
};

static const float village_size = 0.15f;
static const float village_deltas[] = {
    -village_size, -village_size, 0.0f,
		-village_size, -village_size, 2 * village_size,
		-village_size, village_size, 2 * village_size,
		 village_size, village_size, 0.0f,
		-village_size, -village_size, 0.0f,
		-village_size, village_size, 0.0f,
		 village_size, -village_size, 2 * village_size,
		-village_size, -village_size, 0.0f,
		 village_size, -village_size, 0.0f,
		 village_size, village_size, 0.0f,
		 village_size, -village_size, 0.0f,
		-village_size, -village_size, 0.0f,
		-village_size, -village_size, 0.0f,
		-village_size, village_size, 2 * village_size,
		-village_size, village_size, 0.0f,
		 village_size, -village_size, 2 * village_size,
		-village_size, -village_size, 2 * village_size,
		-village_size, -village_size, 0.0f,
		-village_size, village_size, 2 * village_size,
		-village_size, -village_size, 2 * village_size,
		 village_size, -village_size, 2 * village_size,
		 village_size, village_size, 2 * village_size,
		 village_size, -village_size, 0.0f,
		 village_size, village_size, 0.0f,
		 village_size, -village_size, 0.0f,
		 village_size, village_size, 2 * village_size,
		 village_size, -village_size, 2 * village_size,
		 village_size, village_size, 2 * village_size,
		 village_size, village_size, 0.0f,
		-village_size, village_size, 0.0f,
		 village_size, village_size, 2 * village_size,
		-village_size, village_size, 0.0f,
		-village_size, village_size, 2 * village_size,
		 village_size, village_size, 2 * village_size,
		-village_size, village_size, 2 * village_size,
		 village_size, -village_size, 2 * village_size
};

static const float player_colours[] = {
  1.0f, 0.0f, 0.0f,
  0.0f, 0.0f, 1.0f,
  1.0f, 0.6f, 0.0f,
  1.0f, 1.0f, 1.0f
};

static const float village_centers[] = {
  -2.598f, 3.5f, 0.0f,
  -1.732f, 4.0f, 0.0f,
  -0.866f, 3.5f, 0.0f,
  0.0f, 4.0f, 0.0f,
  0.866f, 3.5f, 0.0f,
  1.732f, 4.0f, 0.0f,
  2.598f, 3.5f, 0.0f,

  -3.464f, 2.0f, 0.0f,
  -2.598f, 2.5f, 0.0f,
  -1.732f, 2.0f, 0.0f,
  -0.866f, 2.5f, 0.0f,
  0.0f, 2.0f, 0.0f,
  0.866f, 2.5f, 0.0f,
  1.732f, 2.0f, 0.0f,
  2.598f, 2.5f, 0.0f,
  3.464f, 2.0f, 0.0f,

  -4.33f, 0.5, 0.0f,
  -3.464f, 1.0f, 0.0f,
  -2.598f, 0.5f, 0.0f,
  -1.732f, 1.0f, 0.0f,
  -0.866f, 0.5f, 0.0f,
  0.0f, 1.0f, 0.0f,
  0.866f, 0.5f, 0.0f,
  1.732f, 1.0f, 0.0f,
  2.598f, 0.5f, 0.0f,
  3.464f, 1.0f, 0.0f,
  4.33f, 0.5, 0.0f,

  -4.33f, -0.5, 0.0f,
  -3.464f, -1.0f, 0.0f,
  -2.598f, -0.5f, 0.0f,
  -1.732f, -1.0f, 0.0f,
  -0.866f, -0.5f, 0.0f,
  0.0f, -1.0f, 0.0f,
  0.866f, -0.5f, 0.0f,
  1.732f, -1.0f, 0.0f,
  2.598f, -0.5f, 0.0f,
  3.464f, -1.0f, 0.0f,
  4.33f, -0.5, 0.0f,

  -3.464f, -2.0f, 0.0f,
  -2.598f, -2.5f, 0.0f,
  -1.732f, -2.0f, 0.0f,
  -0.866f, -2.5f, 0.0f,
  0.0f, -2.0f, 0.0f,
  0.866f, -2.5f, 0.0f,
  1.732f, -2.0f, 0.0f,
  2.598f, -2.5f, 0.0f,
  3.464f, -2.0f, 0.0f,

  -2.598f, -3.5f, 0.0f,
  -1.732f, -4.0f, 0.0f,
  -0.866f, -3.5f, 0.0f,
  0.0f, -4.0f, 0.0f,
  0.866f, -3.5f, 0.0f,
  1.732f, -4.0f, 0.0f,
  2.598f, -3.5f, 0.0f,
};

class ActualUI {
public:
  ActualUI(int);
  void run();
private:
  float tile_deltas[6 * 3 * 3];
  float road_deltas[12 * 3 * 3];
  Game game;
};

#endif
