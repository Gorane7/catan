
#include <stdlib.h>
#include <iostream>

#include "player_resources.hpp"


PlayerResources createResources() {
  PlayerResources resources;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    resources.resources[i] = 0;
  }
  for (int i = 0; i < DEVELOPMENT_CARD_TYPE_AMOUNT; i++) {
    resources.developmentCards[i] = 0;
  }
  return resources;
}

int chooseRandomResource(PlayerResources resources) {
  int totalResources = 0;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    totalResources += resources.resources[i];
  }
  if (totalResources == 0) {
    return -1;
  }
  int target = rand() % totalResources;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    target -= resources.resources[i];
    if (target < 0) {
      return i;
    }
  }
  std::cout << "ERROR UNDEFINED BEHAVIOUR INDEX: JFB43T8HJ3480FJ034";
  return -1;
}
