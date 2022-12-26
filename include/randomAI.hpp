#ifndef RANDOM_AI_HPP
#define RANDOM_AI_HPP

#include "action.hpp"
#include "player_resources.hpp"
#include "game_board.hpp"

class RandomAI {
public:
  int freeVillageLocation(GameBoard, int);
  int freeRoadLocation(GameBoard, int, int);
  Action getAction(GameBoard, PlayerResources[]);
  int index;
};

#endif
