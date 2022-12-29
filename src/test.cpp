#include "game_board.hpp"
#include "terminal_ui.hpp"
#include "actual_ui.hpp"
#include "randomAI.hpp"
#include "data_structures.hpp"

#include <iostream>

using namespace std;

void testVillageArrToXYAndBack() {
  for (int i = 0; i < (MAP_WIDTH + 1) * (2 * MAP_WIDTH + 1); i++) {
    VillageAsTileXYConnection village = villageIntToTileXYConnection(i);
    int backI = villageTileXYConnectionToInt(village);
    cout << "Arr: " << i << " -> (" << village.x << ", " << village.y << "), " << village.isUpper << " -> " << backI << "\n";
  }
}

int main(void){
  //testVillageArrToXYAndBack();
  ActualUI ui(4);
  ui.run();
}
