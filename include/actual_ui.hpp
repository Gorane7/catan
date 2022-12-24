#ifndef ACTUAL_UI_HPP
#define ACTUAL_UI_HPP

#include "game.hpp"

static const int tile_amount = 19;
static const int vertex_per_tile = 18;
static const int float_per_tile = vertex_per_tile * 3;
static const int vertex_per_number = 6;
static const int float_per_number = vertex_per_number * 3;
static const float square_height = 0.01f;

static const float tile_deltas[18 * 3] = {
  0.0f, 0.0f, 0.0f,     // center
  -0.866f, 0.5f, 0.0f,  // left top
  0.0f, 1.0f, 0.0f,     // top
  0.0f, 0.0f, 0.0f,     // center
  0.0f, 1.0f, 0.0f,     // top
  0.866f, 0.5f, 0.0f,  // right top
  0.0f, 0.0f, 0.0f,     // center
  0.866f, 0.5f, 0.0f,  // right top
  0.866f, -0.5f, 0.0f,  // right bottom
  0.0f, 0.0f, 0.0f,     // center
  0.866f, -0.5f, 0.0f,  // right bottom
  0.0f, -1.0f, 0.0f,     // bottom
  0.0f, 0.0f, 0.0f,     // center
  0.0f, -1.0f, 0.0f,     // bottom
  -0.866f, -0.5f, 0.0f,  // left bottom
  0.0f, 0.0f, 0.0f,     // center
  -0.866f, -0.5f, 0.0f,  // left bottom
  -0.866f, 0.5f, 0.0f,  // left top
};
static const float tile_colours[6 * 3] = {
  1.0f, 1.0f, 0.5f,
  0.1f, 0.5f, 0.1f,
  0.1f, 1.0f, 0.1f,
  0.9f, 0.9f, 0.1f,
  0.7f, 0.7f, 0.7f,
  0.5f, 0.2f, 0.2f
};

static const float tile_centers[19 * 3] = {
  -1.732f, 3.0f, 0.0f,
  0.0f, 3.0f, 0.0f,
  1.732f, 3.0f, 0.0f,
  -2.598f, 1.5f, 0.0f,
  -0.866f, 1.5f, 0.0f,
  0.866f, 1.5f, 0.0f,
  2.598f, 1.5f, 0.0f,
  -3.464f, 0.0f, 0.0f,
  -1.732f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.0f,
  1.732f, 0.0f, 0.0f,
  3.464f, 0.0f, 0.0f,
  -2.598f, -1.5f, 0.0f,
  -0.866f, -1.5f, 0.0f,
  0.866f, -1.5f, 0.0f,
  2.598f, -1.5f, 0.0f,
  -1.732f, -3.0f, 0.0f,
  0.0f, -3.0f, 0.0f,
  1.732f, -3.0f, 0.0f
};

static const float square_radius = 0.2f;
static const float square_deltas[6 * 3] {
  -square_radius, square_radius, 0.0f,
  square_radius, square_radius, 0.0f,
  -square_radius, -square_radius, 0.0f,
  square_radius, -square_radius, 0.0f,
  square_radius, square_radius, 0.0f,
  -square_radius, -square_radius, 0.0f
};

void run(GameBoard);

#endif
