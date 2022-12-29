#include "game_board.hpp"
#include "data_structures.hpp"

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

bool areNeighbours(int a, int b) {
  VillageAsTileXYConnection villageA = villageIntToTileXYConnection(a);
  VillageAsTileXYConnection villageB = villageIntToTileXYConnection(b);

  if (villageA.isUpper == villageB.isUpper) {
    return false;
  }
  int dx = villageB.x - villageA.x;
  int dy = villageB.y - villageA.y;
  if (villageB.isUpper) {
    dx = -dx;
    dy = -dy;
  } // A is upper after this;
  if (dx == -1 && dy == 2) {
    return true;
  }
  if (dx == 0 && dy == 1) {
    return true;
  }
  if (dx == -1 && dy == 1) {
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

bool villageLocationIsHostile(GameBoard board, int village, int playerID) {
  return board.villages[village] != -1 && board.villages[village] != playerID;
}

bool roadConnectsToPlayersRoad(GameBoard board, int roadLocation, int playerID) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (!villageLocationIsHostile(board, villageA, playerID) && areNeighbours(i, villageA)) {
      if (board.roads[i * VILLAGE_ARRAY_LENGTH + villageA] == playerID) {
        return true;
      }
      if (board.roads[villageA * VILLAGE_ARRAY_LENGTH + i] == playerID) {
        return true;
      }
    }
    if (!villageLocationIsHostile(board, villageB, playerID) && areNeighbours(i, villageB)) {
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

std::vector<int> villagesNextToTile(GameBoard board, int tile) {
  std::vector<int> villages;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (board.villages[i] == -1) {
      continue;
    }
    int player = board.villages[i];
    VillageAsTileXYConnection village = villageIntToTileXYConnection(i);
    int arrs[6];
    if (village.isUpper) {
      arrs[0] = village.x - 1;
      arrs[1] = village.y + 1;
      arrs[2] = village.x;
      arrs[3] = village.y + 1;
      arrs[4] = village.x;
      arrs[5] = village.y;
    } else {
      arrs[0] = village.x + 1;
      arrs[1] = village.y - 1;
      arrs[2] = village.x;
      arrs[3] = village.y - 1;
      arrs[4] = village.x;
      arrs[5] = village.y;
    }
    for (int j = 0; j < 3; j++) {
      int x = arrs[j * 2] + MAP_RADIUS;
      int y = arrs[j * 2 + 1] + MAP_RADIUS;
      if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
        continue;
      }
      int neighbour = y * MAP_WIDTH + x;
      if (!isValidTile(neighbour)) {
        continue;
      }
      villages.push_back(i);
    }
  }
  return villages;
}

bool roadLocationIsEmpty(GameBoard board, int roadLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return board.roads[villageA * VILLAGE_ARRAY_LENGTH + villageB] == -1 && board.roads[villageB * VILLAGE_ARRAY_LENGTH + villageA] == -1;
}

bool isValidRoadLocationForPlayer(GameBoard board, int roadLocation, int playerID) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return areNeighbours(villageA, villageB) && villageOnBoard(villageA) && villageOnBoard(villageB) && (roadConnectsToPlayersRoad(board, roadLocation, playerID) || roadConnectsToPlayersVillage(board, roadLocation, playerID)) && roadLocationIsEmpty(board, roadLocation);
}

bool isValidRoadLocation(int roadLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return areNeighbours(villageA, villageB) && villageOnBoard(villageA) && villageOnBoard(villageB);
}

bool villageLocationNextToPlayersRoad(GameBoard board, int villageLocation, int playerID) {
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (!areNeighbours(i, villageLocation)) {
      continue;
    }
    if (board.roads[i * VILLAGE_ARRAY_LENGTH + villageLocation] == playerID || board.roads[villageLocation * VILLAGE_ARRAY_LENGTH + i] == playerID) {
      return true;
    }
  }
  return false;
}

std::vector<int> availableVillageSpots(GameBoard board, int playerID) {
  std::vector<int> villageSpots;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (isValidVillageLocationForPlayer(board, i, playerID)) {
      villageSpots.push_back(i);
    }
  }
  return villageSpots;
}

bool hasVillageSpot(GameBoard board, int playerID) {
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (isValidVillageLocationForPlayer(board, i, playerID)) {
      return true;
    }
  }
  return false;
}

std::vector<int> availableRoadSpots(GameBoard board, int playerID) {
  std::vector<int> roadSpots;
  for (int i = 0; i < ROAD_ARRAY_LENGTH; i++) {
    if (isValidRoadLocationForPlayer(board, i, playerID)) {
      roadSpots.push_back(i);
    }
  }
  return roadSpots;
}

bool hasRoadSpot(GameBoard board, int playerID) {
  for (int i = 0; i < ROAD_ARRAY_LENGTH; i++) {
    if (isValidRoadLocationForPlayer(board, i, playerID)) {
      return true;
    }
  }
  return false;
}

bool isValidVillageLocationForPlayer(GameBoard board, int villageLocation, int playerID) {
  return isValidVillageLocation(board, villageLocation) && villageLocationNextToPlayersRoad(board, villageLocation, playerID);
}

bool villageOnBoard(int village) {
  return villageOnBoard(village % (MAP_WIDTH * 2 + 1), village / (MAP_WIDTH * 2 + 1));
}

bool villageOnBoard(int x, int y) {
  return !(x + y < 2 || MAP_WIDTH * 2 - x + y < 2 || MAP_WIDTH - y + x < 2 || MAP_WIDTH * 3 - y - x < 2);
}

bool isValidVillageLocation(GameBoard board, int villageLocation) {
  if (board.villages[villageLocation] != -1) {
    return false;
  }
  if (!villageOnBoard(villageLocation)) {
    return false;
  }
  std::vector<int> neighbours = getVillageNeighbours(villageLocation);
  for (int i = 0; i < neighbours.size(); i++) {
    if (board.villages[neighbours[i]] != -1) {
      return false;
    }
  }
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

std::vector<int> availableCityBuildingLocations(GameBoard board, int index) {
  std::vector<int> cityBuildingLocations;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (board.villages[i] == index && board.cities[i] == -1) {
      cityBuildingLocations.push_back(i);
    }
  }
  return cityBuildingLocations;
}

bool hasCityBuildingLocation(GameBoard board, int index) {
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (board.villages[i] == index && board.cities[i] == -1) {
      return true;
    }
  }
  return false;
}

bool isValidCityLocationForPlayer(GameBoard board, int cityLocation, int index) {
  return board.villages[cityLocation] == index && board.cities[cityLocation] == -1;
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
    board.cities[i] = -1;
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

int abstractTileToTile(int abstractTile) {
  int c = 0;
  for (int i = 0; i < MAP_WIDTH * MAP_WIDTH; i++) {
    if (isValidTile(i)) {
      if (c == abstractTile) {
        return i;
      }
      c++;
    }
  }
  return MAP_WIDTH * MAP_WIDTH;
}
