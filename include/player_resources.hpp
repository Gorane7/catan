#ifndef PLAYER_RESOURCES_HPP
#define PLAYER_RESOURCES_HPP

const int RESOURCE_TYPE_AMOUNT = 5;
const int WOOD = 0;
const int SHEEP = 1;
const int WHEAT = 2;
const int ROCK = 3;
const int CLAY = 4;

struct PlayerResources {
  int resources[RESOURCE_TYPE_AMOUNT];
};

PlayerResources createResources();

#endif
