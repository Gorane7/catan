
#include "randomAI.hpp"

#include <stdlib.h>
#include <ctime>
#include <iostream>

int RandomAI::freeVillageLocation(GameBoard board, int myIndex) {
  srand (static_cast <unsigned> (time(0)));
  int answer = rand() % VILLAGE_LOCATION_AMOUNT;
  std::cout << "Player with index " << myIndex << " was asked where to place their village and the answer was " << answer << "\n";
  return answer;
}
