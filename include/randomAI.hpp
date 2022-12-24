#ifndef RANDOM_AI_HPP
#define RANDOM_AI_HPP

#include "player.hpp"

class RandomAI: public Player {
public:
  int freeVillageLocation(GameBoard, int);
};

#endif
