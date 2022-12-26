#include "game_board.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

GameBoard randomBoard(int playerAmount) {
  return randomBoard(playerAmount, false);
}

int villageArrayLocation(bool isUpper, int x, int y) {
  int tileRow = y + 2;
  int tileColumn = x + (tileRow + 1) / 2 - (MAP_RADIUS - 1) + MAP_RADIUS;
  int villageY = tileRow;
  int villageX = 2 * tileColumn;
  if (isUpper) {
    villageY++;
    if ((villageX + villageY) % 2 == 1) {
      villageX++;
    }
  } else {
    if ((villageX + villageY) % 2 != 1) {
      villageX++;
    }
  }
  return villageY * (MAP_WIDTH * 2 + 1) + villageX;
}

bool villageArrToUpperBool(int i) {
  return (i % (MAP_WIDTH * 2 + 1) + i / (MAP_WIDTH * 2 + 1)) % 2 == 0;
}

int villageArrToTileX(int i) {
  int villageX = i % (MAP_WIDTH * 2 + 1);
  int villageY = i / (MAP_WIDTH * 2 + 1);
  int tileColumn = villageX / 2;
  int tileRow = villageY;
  if ((villageX + villageY) % 2 == 0) {
    tileRow--;
  }
  return tileColumn - (tileRow + 1) / 2 + (MAP_RADIUS - 1) - MAP_RADIUS;
}

int villageArrToTileY(int i) {
  int villageX = i % (MAP_WIDTH * 2 + 1);
  int villageY = i / (MAP_WIDTH * 2 + 1);
  int tileRow = villageY;
  if ((villageX + villageY) % 2 == 0) {
    tileRow--;
  }
  return tileRow - 2;
}

bool areNeighbours(int a, int b) {
  int tileXa = villageArrToTileX(a);
  int tileYa = villageArrToTileY(a);
  bool isUpperA = villageArrToUpperBool(a);

  int tileXb = villageArrToTileX(b);
  int tileYb = villageArrToTileY(b);
  bool isUpperB = villageArrToUpperBool(b);

  if (isUpperA == isUpperB) {
    return false;
  }
  if (isUpperB) {
    int temp = tileXa;
    tileXa = tileXb;
    tileXb = temp;
    temp = tileYa;
    tileYa = tileYb;
    tileYb = temp;
  } // A is upper after this;
  int dx = tileXb - tileXa;
  int dy = tileYb - tileYa;
  if (dx == -1 && dy == 2) {
    std::cout << "Vertical road\n";
    return true;
  }
  if (dx == 0 && dy == 1) {
    std::cout << "Down right road\n";
    return true;
  }
  if (dx == -1 && dy == 1) {
    std::cout << "Down left road\n";
    return true;
  }
  return false;
}

bool isValidRoadLocationNextToVillage(GameBoard board, int roadLocation, int playerID, int villageLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return villageOnBoard(villageA) && villageOnBoard(villageB) && areNeighbours(villageA, villageB) && (villageA == villageLocation || villageB == villageLocation);
}

bool roadConnectsToPlayersVillage(GameBoard board, int roadLocation, int playerID) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return board.villages[villageA] == playerID || board.villages[villageB] == playerID;
}

bool roadConnectsToPlayersRoad(GameBoard board, int roadLocation, int playerID) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (areNeighbours(i, villageA)) {
      if (board.roads[i * VILLAGE_ARRAY_LENGTH + villageA] == playerID) {
        return true;
      }
      if (board.roads[villageA * VILLAGE_ARRAY_LENGTH + i] == playerID) {
        return true;
      }
    }
    if (areNeighbours(i, villageB)) {
      if (board.roads[i * VILLAGE_ARRAY_LENGTH + villageB] == playerID) {
        return true;
      }
      if (board.roads[villageB * VILLAGE_ARRAY_LENGTH + i] == playerID) {
        return true;
      }
    }
  }
  return false;
}

bool roadLocationIsEmpty(GameBoard board, int roadLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return board.roads[villageA * VILLAGE_ARRAY_LENGTH + villageB] == -1 && board.roads[villageB * VILLAGE_ARRAY_LENGTH + villageA] == -1;
}

bool isValidRoadLocationForPlayer(GameBoard board, int roadLocation, int playerID) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return (roadConnectsToPlayersRoad(board, roadLocation, playerID) || roadConnectsToPlayersVillage(board, roadLocation, playerID)) && roadLocationIsEmpty(board, roadLocation) && areNeighbours(villageA, villageB);
}

bool villageOnBoard(int village) {
  return villageOnBoard(village % (MAP_WIDTH * 2 + 1), village / (MAP_WIDTH * 2 + 1));
}

bool villageOnBoard(int x, int y) {
  return !(x + y < 2 || MAP_WIDTH * 2 - x + y < 2 || MAP_WIDTH - y + x < 2 || MAP_WIDTH * 3 - y - x < 2);
}

bool isValidVillageLocation(GameBoard board, int villageLocation, int playerID) {
  std::cout << "Was asked if " << villageLocation << " is a valid location for player " << playerID << "\n";
  if (board.villages[villageLocation] != -1) {
    std::cout << "There is already a village from player " << board.villages[villageLocation] << " there\n";
    return false;
  }
  int villageX = villageLocation % (MAP_WIDTH * 2 + 1);
  int villageY = villageLocation / (MAP_WIDTH * 2 + 1);
  if (!villageOnBoard(villageX, villageY)) {
    std::cout << "Village is so far in corner that it is out of map\n";
    return false;
  }
  int tileX = villageArrToTileX(villageLocation);
  int tileY = villageArrToTileY(villageLocation);
  bool isUpper = villageArrToUpperBool(villageLocation);
  int arrs[3];
  if (isUpper) {
    arrs[0] = villageArrayLocation(false, tileX - 1, tileY + 1);
    arrs[1] = villageArrayLocation(false, tileX, tileY + 1);
    arrs[2] = villageArrayLocation(false, tileX - 1, tileY + 2);
  } else {
    arrs[0] = villageArrayLocation(true, tileX + 1, tileY - 1);
    arrs[1] = villageArrayLocation(true, tileX, tileY - 1);
    arrs[2] = villageArrayLocation(true, tileX + 1, tileY - 2);
  }
  for (int i = 0; i < 3; i++) {
    if (arrs[i] < 0 || arrs[i] >= VILLAGE_ARRAY_LENGTH) {
      continue;
    }
    if (board.villages[arrs[i]] != -1) {
      std::cout << "Someone's village was already next to " << villageLocation << "\n";
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
  board.roadAmount = 0;

  for (int i = 0; i < ROAD_ARRAY_LENGTH; i++) {
    board.roads[i] = -1;
  }

  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    board.villages[i] = -1;
  }

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
  for (int i = 0; i < MAP_WIDTH * MAP_WIDTH; i++) {
    if (isValidTile(i)) {
      board.tiles[i] = board.abstractTiles[c];
      c++;
    } else {
      board.tiles[i] = -1;
    }
  }
  return board;
}

int tileToAbstractTile(int tile) {
  int c = 0;
  for (int i = 0; i < tile; i++) {
    if (isValidTile(i)) {
      c++;
    }
  }
  return c;
}
