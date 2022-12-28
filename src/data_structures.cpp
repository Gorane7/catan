
#include "data_structures.hpp"
#include "constants.hpp"

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
