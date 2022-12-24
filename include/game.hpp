#ifndef GAME_HPP
#define GAME_HPP

#include "game_board.hpp"
#include "player.hpp"
#include "randomAI.hpp"

static const int MAX_PLAYERS = 4;

struct Game {
  int playerAmount;
  GameBoard board;
  RandomAI players[MAX_PLAYERS];
  int currentTurn;
};

Game createGame(int);
void askAction(Game&);

#endif
