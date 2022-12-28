
#include "randomAI.hpp"
#include "action.hpp"
#include "player_resources.hpp"

#include <stdlib.h>
#include <ctime>
#include <iostream>
#include <vector>


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
  int tradeFrom = rand() % RESOURCE_TYPE_AMOUNT;
  if (myResources.resources[tradeFrom] >= 4) {
    action.actionType = TRADE_RESOURCES;
    action.resourceFrom = tradeFrom;
    action.resourceTo = rand() % RESOURCE_TYPE_AMOUNT;
  }
  std::vector<int> resourcesThatHasMany;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    if (myResources.resources[i] >= 7) {
      resourcesThatHasMany.push_back(i);
    }
  }
  if (resourcesThatHasMany.size() > 0) {
    int tradeFromIndex = rand() % resourcesThatHasMany.size();
    action.actionType = TRADE_RESOURCES;
    action.resourceFrom = resourcesThatHasMany[tradeFromIndex];
    action.resourceTo = rand() % RESOURCE_TYPE_AMOUNT;
  }
  if (myResources.resources[WOOD] >= 1 && myResources.resources[CLAY] >= 1) {
    std::vector<int> roadSpots = availableRoadSpots(board, index);
    if (roadSpots.size() > 0) {
      action.actionType = BUILD_ROAD;
      action.actionLocation = roadSpots[rand() % roadSpots.size()];
    }
  }
  if (myResources.resources[WOOD] >= 1 && myResources.resources[CLAY] >= 1 && myResources.resources[SHEEP] >= 1 && myResources.resources[WHEAT] >= 1) {
    std::vector<int> villageSpots = availableVillageSpots(board, index);
    if (villageSpots.size() > 0) {
      action.actionType = BUILD_VILLAGE;
      action.actionLocation = villageSpots[rand() % villageSpots.size()];
    }
  }
  if (myResources.resources[WHEAT] >= 2 && myResources.resources[ROCK] >= 3) {
    std::vector<int> citySpots = availableCityBuildingLocations(board, index);
    if (citySpots.size() > 0) {
      action.actionType = BUILD_CITY;
      action.actionLocation = citySpots[rand() % citySpots.size()];
    }
  }
  return action;
}
