
#include "randomAI.hpp"

#include <stdlib.h>
#include <ctime>
#include <iostream>

int RandomAI::freeVillageLocation(GameBoard board, int myIndex) {
  int answer = rand() % (MAP_WIDTH * MAP_WIDTH * 2);
  std::cout << "Player with index " << myIndex << " was asked where to place their village and the answer was " << answer << "\n";
  return answer;
}
