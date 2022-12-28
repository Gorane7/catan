
#include "data_structures.hpp"
#include "constants.hpp"

VillageAsTileXYConnection villageIntToTileXYConnection(int i) {
  VillageAsTileXYConnection converted;
  int villageX = i % (MAP_WIDTH * 2 + 1);
  int villageY = i / (MAP_WIDTH * 2 + 1);
  int tileColumn = villageX / 2;
  int tileRow = villageY;
  converted.isUpper = (villageX + villageY) % 2 == 0;
  if (converted.isUpper) {
    tileRow--;
  }
  converted.x = tileColumn - (tileRow + 1) / 2 + (MAP_RADIUS - 1) - MAP_RADIUS;
  converted.y = tileRow - 2;
  return converted;
}
