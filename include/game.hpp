#ifndef GAME_HPP
#define GAME_HPP

#include <string>

const int TILE_AMOUNT = 19;
const int FOREST_AMOUNT = 4;
const int PASTURE_AMOUNT = 4;
const int FIELD_AMOUNT = 4;
const int MOUNTAIN_AMOUNT = 3;
const int QUARRY_AMOUNT = 3;
const int NUMBER_AMOUNT = FOREST_AMOUNT + PASTURE_AMOUNT + FIELD_AMOUNT + MOUNTAIN_AMOUNT + QUARRY_AMOUNT;
const int DESERT_AMOUNT = TILE_AMOUNT - NUMBER_AMOUNT;
const int TILE_TYPE_AMOUNT = 6;

// Following code block contains information that needs to match each other and previous information
const int NUMBERS[NUMBER_AMOUNT] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
const int NEIGHBOURS[TILE_AMOUNT * 6] = {
  1, 3, 4, -1, -1, -1,
  0, 2, 4, 5, -1, -1,
  1, 5, 6, -1, -1, -1,
  0, 4, 7, 8, -1, -1,
  0, 1, 3, 5, 8, 9,
  1, 2, 4, 6, 9, 10,
  2, 5, 10, 11, -1, -1,
  3, 8, 12, 13, -1, -1,
  3, 4, 7, 9, 12, 13,
  4, 5, 8, 10, 13, 14,
  5, 6, 9, 11, 14, 15,
  6, 10, 15, -1, -1, -1,
  7, 8, 13, 16, -1, -1,
  8, 9, 12, 14, 16, 17,
  9, 10, 13, 15, 17, 18,
  10, 11, 14, 18, -1, -1,
  12, 13, 17, -1, -1, -1,
  13, 14, 16, 18, -1, -1,
  14, 15, 17, -1, -1, -1
};
const int TILE_AMOUNTS[TILE_TYPE_AMOUNT] = {DESERT_AMOUNT, FOREST_AMOUNT, PASTURE_AMOUNT, FIELD_AMOUNT, MOUNTAIN_AMOUNT, QUARRY_AMOUNT};
const std::string TILE_NAMES[TILE_TYPE_AMOUNT] = {"Desert", "Forest", "Pasture", "Field", "Mountain", "Quarry"};
const int DESERT = 0;
const int FOREST = 1;
const int PASTURE = 2;
const int FIELD = 3;
const int MOUNTAIN = 4;
const int QUARRY = 5;

struct GameBoard {
  int tiles[TILE_AMOUNT];
  int numbers[NUMBER_AMOUNT];
  int robberLocation;
};

GameBoard randomBoard();
GameBoard randomBoard(bool);

#endif
