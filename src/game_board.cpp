#include "game_board.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

GameBoard randomBoard(int playerAmount) {
  return randomBoard(playerAmount, false);
}

bool isValidVillageLocation(GameBoard board, int villageLocation, int playerID) {
  std::cout << "Was asked if " << villageLocation << " is a valid location for player " << playerID << "\n";
  if (board.villages[villageLocation] != -1) {
    std::cout << "There is already a village from player " << board.villages[villageLocation] << " there\n";
    return false;
  }
  for (int i = 0; i < 3; i++) {
    if (NEIGHBOUR_VILLAGES[villageLocation * 3 + i] == -1) {
      break;
    }
    if (board.villages[NEIGHBOUR_VILLAGES[villageLocation * 3 + i]] != -1) {
      std::cout << "There is already a village from player " << board.villages[NEIGHBOUR_VILLAGES[villageLocation * 3 + i]] << " in neighbouring location: " << NEIGHBOUR_VILLAGES[villageLocation * 3 + i] << "\n";
      return false;
    }
  }
  std::cout << "Let's hope so\n";
  return true;
}

int numberAtTile(GameBoard board, int tile) {
  if (board.abstractTiles[tile] == DESERT) {
    return -1;
  }
  int desertsFound = 0;
  for (int i = 0; i < tile; i++) {
    if (board.abstractTiles[i] == DESERT) {
      desertsFound += 1;
    }
  }
  return board.numbers[tile - desertsFound];
}

bool isValidTile(int i) {
  int x = i % MAP_WIDTH - MAP_RADIUS;
  int y = i / MAP_WIDTH - MAP_RADIUS;
  return !(x + y > MAP_RADIUS || x + y < - MAP_RADIUS);
}

GameBoard randomBoard(int playerAmount, bool numbersAreIndex) {
  GameBoard board;
  srand((unsigned)time(0));

  board.playerAmount = playerAmount;
  board.villageAmount = 0;
  for (int i = 0; i < VILLAGE_LOCATION_AMOUNT; i++) {
    board.villages[i] = -1;
  }

  // Prefill tile array
  int i = 0;
  for (int j = 0; j < TILE_TYPE_AMOUNT; j++) {
    for (int k = 0; k < TILE_AMOUNTS[j]; k++) {
      board.abstractTiles[i] = j;
      i++;
    }
  }
  // Shuffle tile array
  for (int i = TILE_AMOUNT - 1; i >= 0; i--) {
    int takeFrom = rand() % (i + 1);
    int temp = board.abstractTiles[takeFrom];
    board.abstractTiles[takeFrom] = board.abstractTiles[i];
    board.abstractTiles[i] = temp;
  }
  // Find desert and therefore robber location
  for (int i = 0; i < TILE_AMOUNT; i++) {
    if (board.abstractTiles[i] == DESERT) {
      if (numbersAreIndex) {
        board.abstractTiles[i] = board.abstractTiles[18];
        board.abstractTiles[18] = DESERT;
      }
      board.robberLocation = i;
      break;
    }
  }

  // Prefill number array
  for (int i = 0; i < NUMBER_AMOUNT; i++) {
    board.numbers[i] = NUMBERS[i];
  }

  // Shuffle number array until no 6 or 8 is next to each other
  bool sixOrEightNeighbours;
  do {
    for (int i = NUMBER_AMOUNT - 1; i >= 0; i--) {
      int takeFrom = rand() % (i + 1);
      int temp = board.numbers[takeFrom];
      board.numbers[takeFrom] = board.numbers[i];
      board.numbers[i] = temp;
    }
    sixOrEightNeighbours = false;
    for (int i = 0; i < TILE_AMOUNT; i++) {
      if (i == board.robberLocation) {
        continue;
      }
      int thisNumber = numberAtTile(board, i);
      if (thisNumber != 6 && thisNumber != 8) {
        continue;
      }
      for (int j = 0; j < 6; j++) {
        if (NEIGHBOUR_TILES[i * 6 + j] == -1) {
          continue;
        }
        int otherNumber = numberAtTile(board, NEIGHBOUR_TILES[i * 6 + j]);
        if (otherNumber == 6 || otherNumber == 8) {
          sixOrEightNeighbours = true;
          break;
        }
      }
    }
  } while (sixOrEightNeighbours);

  // If indexes should be displayed on board, then overwrite numbers
  if (numbersAreIndex) {
    board.robberLocation = 18;
    for (int i = 0; i < NUMBER_AMOUNT; i++) {
      board.numbers[i] = i;
    }
  }

  int c = 0;
  for (int i = 0; i < MAP_RADIUS * MAP_RADIUS; i++) {
    if (isValidTile(i)) {
      board.tiles[i] = board.abstractTiles[c];
      c++;
    } else {
      board.tiles[i] = -1;
    }
  }
  return board;
}
