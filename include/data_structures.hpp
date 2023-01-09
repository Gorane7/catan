#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <vector>
#include <array>

struct VillageAsTileXYConnection {
  int x;
  int y;
  bool isUpper;
};

struct VillageAsGridXY {
  int x;
  int y;
};

struct VillageAsInt {
  int i;
};

VillageAsTileXYConnection villageIntToTileXYConnection(int);
int villageTileXYConnectionToInt(VillageAsTileXYConnection);

VillageAsGridXY villageIntToGridXY(int);
int villageGridXYToInt(VillageAsGridXY);

VillageAsInt villageIntToCheckedInt(int);
int villageCheckedIntToInt(VillageAsInt);

std::vector<int> getVillageNeighbours(VillageAsInt);
bool areVillagesNeighbours(int, int);

bool isValidRoadLocationNextToVillage(int, int, int);
bool villageOnBoard(int, int);
bool villageOnBoard(int);
bool isValidRoadLocation(int);
bool isValidTile(int);
int tileToAbstractTile(int);
int abstractTileToTile(int);
bool isValidRoad(int);
int roadToAbstractRoad(int);
int abstractRoadToRoad(int);

VillageAsTileXYConnection getNorthVillageNeighbour(VillageAsTileXYConnection);
VillageAsTileXYConnection getSouthEastVillageNeighbour(VillageAsTileXYConnection);
VillageAsTileXYConnection getSouthWestVillageNeighbour(VillageAsTileXYConnection);
VillageAsTileXYConnection getSouthVillageNeighbour(VillageAsTileXYConnection);
VillageAsTileXYConnection getNorthEastVillageNeighbour(VillageAsTileXYConnection);
VillageAsTileXYConnection getNorthWestVillageNeighbour(VillageAsTileXYConnection);

#endif
