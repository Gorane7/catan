#include "game_board.hpp"
#include "terminal_ui.hpp"
#include "actual_ui.hpp"
#include "randomAI.hpp"

#include <iostream>

using namespace std;

void testVillageArrToXYAndBack() {
  for (int i = 0; i < (MAP_WIDTH + 1) * (2 * MAP_WIDTH + 1); i++) {
    int x = villageArrToTileX(i);
    int y = villageArrToTileY(i);
    bool isUpper = villageArrToUpperBool(i);
    int backI = villageArrayLocation(isUpper, x, y);
    cout << "Arr: " << i << " -> (" << x << ", " << y << "), " << isUpper << " -> " << backI << "\n";
  }
}

int main(void){
  //testVillageArrToXYAndBack();
  run();
}
