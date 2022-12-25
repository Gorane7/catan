#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include <string>

const int MAP_RADIUS = 2;
const int MAP_WIDTH = MAP_RADIUS * 2 + 1;
const int TILE_AMOUNT = 19;
const int FOREST_AMOUNT = 4;
const int PASTURE_AMOUNT = 4;
const int FIELD_AMOUNT = 4;
const int MOUNTAIN_AMOUNT = 3;
const int QUARRY_AMOUNT = 3;
const int NUMBER_AMOUNT = FOREST_AMOUNT + PASTURE_AMOUNT + FIELD_AMOUNT + MOUNTAIN_AMOUNT + QUARRY_AMOUNT;
const int DESERT_AMOUNT = TILE_AMOUNT - NUMBER_AMOUNT;
const int TILE_TYPE_AMOUNT = 6;
const int VILLAGE_LOCATION_AMOUNT = 54;

// Following code block contains information that needs to match each other and previous information
const int NUMBERS[NUMBER_AMOUNT] = {2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
const int NEIGHBOUR_TILES[TILE_AMOUNT * 6] = {
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

const int NEIGHBOUR_VILLAGES[VILLAGE_LOCATION_AMOUNT * 3] = {
  1, 8, -1, // 0
  0, 2, -1, // 1
  1, 3, 10, // 2
  2, 4, -1, // 3
  3, 5, 12, // 4
  4, 6, -1, // 5
  5, 14, -1, // 6
  8, 17, -1, // 7
  0, 7, 9, // 8
  8, 10, 19, // 9
  2, 9, 11, // 10
  10, 12, 21, // 11
  11, 13, 4, // 12
  12, 14, 23, // 13
  6, 13, 15, // 14
  14, 25, -1, // 15
  17, 27, -1, // 16
  7, 16, 18, // 17
  17, 19, 29, // 18
  9, 18, 20, // 19
  19, 21, 31, // 20
  11, 20, 22, // 21
  21, 23, 33, // 22
  13, 22, 24, // 23
  23, 25, 35, // 24
  15, 24, 26, // 25
  25, 37, -1, // 26
  16, 28, -1, // 27
  27, 29, 38, // 28
  18, 28, 30, // 29
  29, 31, 40, // 30
  20, 30, 32, // 31
  31, 33, 42, // 32
  22, 32, 34, // 33
  33, 35, 44, // 34
  24, 34, 36, // 35
  35, 37, 46, // 36
  26, 36, -1, // 37
  28, 39, -1, // 38
  28, 40, 47, // 39
  30, 39, 41, // 40
  40, 42, 49, // 41
  32, 41, 43, // 42
  42, 44, 51, // 43
  43, 34, 45, // 44
  44, 46, 53, // 45
  36, 45, -1, // 46
  39, 48, -1, // 47
  47, 49, -1, // 48
  41, 48, 50, // 49
  49, 51, -1, // 50
  43, 50, 52, // 51
  51, 53, -1, // 52
  45, 52, -1 // 53
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
  int abstractTiles[TILE_AMOUNT];
  int tiles[MAP_WIDTH * MAP_WIDTH];
  int numbers[NUMBER_AMOUNT];
  int robberLocation;
  int villages[VILLAGE_LOCATION_AMOUNT];
  int playerAmount;
  int villageAmount;
};

int numberAtTile(GameBoard, int);
GameBoard randomBoard(int);
GameBoard randomBoard(int, bool);
bool isValidVillageLocation(GameBoard, int, int);
bool isValidTile(int);

#endif
