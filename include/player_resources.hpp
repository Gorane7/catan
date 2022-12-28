#ifndef PLAYER_RESOURCES_HPP
#define PLAYER_RESOURCES_HPP

const int RESOURCE_TYPE_AMOUNT = 5;
const int WOOD = 0;
const int SHEEP = 1;
const int WHEAT = 2;
const int ROCK = 3;
const int CLAY = 4;

const int DEVELOPMENT_CARD_TYPE_AMOUNT = 5;
const int KNIGHT_CARD = 0;
const int VICTORY_POINT_CARD = 1;
const int ROAD_BUILDING_CARD = 2;
const int YEAR_OF_PLENTY_CARD = 3;
const int MONOPOLY_CARD = 4;

struct PlayerResources {
  int resources[RESOURCE_TYPE_AMOUNT];
  int developmentCards[DEVELOPMENT_CARD_TYPE_AMOUNT];
};

PlayerResources createResources();
int chooseRandomResource(PlayerResources);

#endif
