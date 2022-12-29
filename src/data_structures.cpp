
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

std::vector<int> getVillageNeighbours(int villageLocation) {
  VillageAsTileXYConnection village = villageIntToTileXYConnection(villageLocation);
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
