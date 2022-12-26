#ifndef GAME_HPP
#define GAME_HPP

#include "game_board.hpp"
#include "player.hpp"
#include "randomAI.hpp"
#include "player_resources.hpp"

static const int MAX_PLAYERS = 4;

struct Game {
  int playerAmount;
  GameBoard board;
  RandomAI players[MAX_PLAYERS];
  int currentTurn;
  PlayerResources resources[MAX_PLAYERS];
};

Game createGame(int);
void askAction(Game&);

#endif
