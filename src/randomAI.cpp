
#include "randomAI.hpp"
#include "action.hpp"
#include "player_resources.hpp"

#include <stdlib.h>
#include <ctime>
#include <iostream>

int RandomAI::freeVillageLocation(GameBoard board, int myIndex) {
  int answer = rand() % VILLAGE_ARRAY_LENGTH;
  std::cout << "Player with index " << myIndex << " was asked where to place their village and the answer was " << answer << "\n";
  return answer;
}

int RandomAI::freeRoadLocation(GameBoard board, int myIndex, int nextToVillage) {
  int answer = rand() % ROAD_ARRAY_LENGTH;
  //std::cout << "PLayer with index " << myIndex << " was asked to place a road next to village " << nextToVillage << " and the answer was " << answer << "\n";
  return answer;
}

Action RandomAI::getAction(GameBoard board, PlayerResources resources[]) {
  Action action;
  PlayerResources myResources = resources[index];
  action.actionType = END_TURN;
  if (myResources.resources[WOOD] >= 1 && myResources.resources[CLAY] >= 1) {
    action.actionType = BUILD_ROAD;
    action.actionLocation = rand() % ROAD_ARRAY_LENGTH;
  }
  return action;
}
