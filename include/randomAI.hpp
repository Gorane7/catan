#ifndef RANDOM_AI_HPP
#define RANDOM_AI_HPP

#include "player.hpp"
#include "action.hpp"

class RandomAI: public Player {
public:
  int freeVillageLocation(GameBoard, int);
  int freeRoadLocation(GameBoard, int, int);
  Action getAction(GameBoard);
};

#endif
