#ifndef PLAYER_RESOURCES_HPP
#define PLAYER_RESOURCES_HPP

const int RESOURCE_TYPE_AMOUNT = 5;

struct PlayerResources {
  int resources[RESOURCE_TYPE_AMOUNT];
};

PlayerResources createResources();

#endif
