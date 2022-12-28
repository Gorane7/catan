#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

struct VillageAsTileXYConnection {
  int x;
  int y;
  bool isUpper;
};

VillageAsTileXYConnection villageIntToTileXYConnection(int);

#endif
