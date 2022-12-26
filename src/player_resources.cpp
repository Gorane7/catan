
#include "player_resources.hpp"


PlayerResources createResources() {
  PlayerResources resources;
  for (int i = 0; i < RESOURCE_TYPE_AMOUNT; i++) {
    resources.resources[i] = 0;
  }
  return resources;
}
