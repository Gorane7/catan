
#include "data_structures.hpp"
#include "constants.hpp"

VillageAsGridXY villageIntToGridXY(int i) {
  VillageAsGridXY village;
  village.x = i % MAP_WIDTH - MAP_RADIUS;
  village.y = i / MAP_WIDTH - MAP_RADIUS;
  return village;
}

int villageGridXYToInt(VillageAsGridXY village) {
  return (village.y + MAP_RADIUS) * MAP_WIDTH + village.x + MAP_RADIUS;
}

VillageAsTileXYConnection villageIntToTileXYConnection(int i) {
  VillageAsTileXYConnection converted;
  int villageX = i % MAP_VILLAGE_WIDTH;
  int tileColumn = villageX / 2;
  int tileRow = i / MAP_VILLAGE_WIDTH;
  converted.isUpper = (villageX + tileRow + 1) % 2;
  if (converted.isUpper) {
    tileRow--;
  }
  converted.x = tileColumn - (tileRow + 1) / 2 - 1;
  converted.y = tileRow - 2;
  return converted;
}

int villageTileXYConnectionToInt(VillageAsTileXYConnection village) {
  int tileRow = village.y + 2;
  int tileColumn = village.x + (tileRow + 1) / 2 - (MAP_RADIUS - 1) + MAP_RADIUS;
  int villageY = tileRow;
  int villageX = 2 * tileColumn;
  if (village.isUpper) {
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

VillageAsInt villageIntToCheckedInt(int i) {
  VillageAsInt village;
  village.i = i;
  return village;
}

int villageCheckedIntToInt(VillageAsInt village) {
  return village.i;
}

bool villageOnBoard(int x, int y) {
  return !(x + y < 2 || MAP_WIDTH * 2 - x + y < 2 || MAP_WIDTH - y + x < 2 || MAP_WIDTH * 3 - y - x < 2);
}

bool villageOnBoard(int village) {
  return villageOnBoard(village % MAP_VILLAGE_WIDTH, village / MAP_VILLAGE_WIDTH);
}

bool isValidRoadLocationNextToVillage(int roadLocation, int playerID, int villageLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return villageOnBoard(villageA) && villageOnBoard(villageB) && areVillagesNeighbours(villageA, villageB) && (villageA == villageLocation || villageB == villageLocation);
}

bool areVillagesNeighbours(int a, int b) {
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

bool isValidTile(int i) {
  int x = i % MAP_WIDTH - MAP_RADIUS;
  int y = i / MAP_WIDTH - MAP_RADIUS;
  return !(x + y > MAP_RADIUS || x + y < - MAP_RADIUS);
}

bool isValidRoadLocation(int roadLocation) {
  int villageA = roadLocation / VILLAGE_ARRAY_LENGTH;
  int villageB = roadLocation % VILLAGE_ARRAY_LENGTH;
  return areVillagesNeighbours(villageA, villageB) && villageOnBoard(villageA) && villageOnBoard(villageB);
}

std::vector<int> getVillageNeighbours(VillageAsInt villageLocation) {
  VillageAsTileXYConnection village = villageIntToTileXYConnection(villageLocation.i);
  std::array<int, 3> neighbours;
  if (village.isUpper) {
    neighbours[0] = villageTileXYConnectionToInt(getSouthWestVillageNeighbour(village));
    neighbours[1] = villageTileXYConnectionToInt(getSouthEastVillageNeighbour(village));
    neighbours[2] = villageTileXYConnectionToInt(getNorthVillageNeighbour(village));
  } else {
    neighbours[0] = villageTileXYConnectionToInt(getNorthEastVillageNeighbour(village));
    neighbours[1] = villageTileXYConnectionToInt(getNorthWestVillageNeighbour(village));
    neighbours[2] = villageTileXYConnectionToInt(getSouthVillageNeighbour(village));
  }
  std::vector<int> trueNeighbours;
  for (int i = 0; i < 3; i++) {
    if (neighbours[i] < 0 || neighbours[i] >= VILLAGE_ARRAY_LENGTH) {
      continue;
    }
    trueNeighbours.push_back(neighbours[i]);
  }
  return trueNeighbours;
}

VillageAsTileXYConnection getNorthVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = false;
  village.x -= 1;
  village.y += 2;
  return village;
}

VillageAsTileXYConnection getSouthEastVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = false;
  village.y += 1;
  return village;
}

VillageAsTileXYConnection getSouthWestVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = false;
  village.x -= 1;
  village.y += 1;
  return village;
}

VillageAsTileXYConnection getSouthVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = true;
  village.x += 1;
  village.y -= 2;
  return village;
}

VillageAsTileXYConnection getNorthWestVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = true;
  village.y -= 1;
  return village;
}

VillageAsTileXYConnection getNorthEastVillageNeighbour(VillageAsTileXYConnection village) {
  village.isUpper = true;
  village.x += 1;
  village.y -= 1;
  return village;
}

/*if (village.isUpper) {
  arrs[0] = villageArrayLocation(false, village.x - 1, village.y + 1);
  arrs[1] = villageArrayLocation(false, village.x, village.y + 1);
  arrs[2] = villageArrayLocation(false, village.x - 1, village.y + 2);
} else {
  arrs[0] = villageArrayLocation(true, village.x + 1, village.y - 1);
  arrs[1] = villageArrayLocation(true, village.x, village.y - 1);
  arrs[2] = villageArrayLocation(true, village.x + 1, village.y - 2);
}*/
