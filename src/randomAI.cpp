
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

int RandomAI::moveRobber(GameBoard board, PlayerResources resources[]) {
  return rand() % TILE_AMOUNT;
}

std::vector<int> RandomAI::discardResources(GameBoard board, PlayerResources resources[], int amountToDiscard) {
  std::vector<int> toDiscard;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    toDiscard.push_back(0);
  }
  int totalDiscarded = 0;
  while (totalDiscarded < amountToDiscard) {
    int target = rand() % RESOURCE_TYPE_AMOUNT;
    if (resources[index].resources[target] > toDiscard[target]) {
      toDiscard[target]++;
      totalDiscarded++;
    }
  }
  return toDiscard;
}

Action RandomAI::getAction(GameBoard board, PlayerResources resources[]) {
  Action action;
  PlayerResources myResources = resources[index];

  if (myResources.resources[WHEAT] >= 2 && myResources.resources[ROCK] >= 3) {
    std::vector<int> citySpots = availableCityBuildingLocations(board, index);
    if (citySpots.size() > 0) {
      action.actionType = BUILD_CITY;
      action.actionLocation = citySpots[rand() % citySpots.size()];
      return action;
    }
  }
  if (myResources.resources[WOOD] >= 1 && myResources.resources[CLAY] >= 1 && myResources.resources[SHEEP] >= 1 && myResources.resources[WHEAT] >= 1) {
    std::vector<int> villageSpots = availableVillageSpots(board, index);
    if (villageSpots.size() > 0) {
      action.actionType = BUILD_VILLAGE;
      action.actionLocation = villageSpots[rand() % villageSpots.size()];
      return action;
    }
  }
  if (myResources.resources[WOOD] >= 1 && myResources.resources[CLAY] >= 1) {
    std::vector<int> roadSpots = availableRoadSpots(board, index);
    if (roadSpots.size() > 0) {
      action.actionType = BUILD_ROAD;
      action.actionLocation = roadSpots[rand() % roadSpots.size()];
      return action;
    }
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
    return action;
  }
  int tradeFrom = rand() % RESOURCE_TYPE_AMOUNT;
  if (myResources.resources[tradeFrom] >= 4) {
    action.actionType = TRADE_RESOURCES;
    action.resourceFrom = tradeFrom;
    action.resourceTo = rand() % RESOURCE_TYPE_AMOUNT;
  }
  action.actionType = END_TURN;
  return action;
}
