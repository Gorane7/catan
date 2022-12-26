#ifndef GAME_HPP
#define GAME_HPP

#include <vector>

#include "game_board.hpp"
#include "player.hpp"
#include "randomAI.hpp"
#include "player_resources.hpp"

static const int MAX_PLAYERS = 4;

// Int to action mappings
static const int VILLAGE_PLACED = 0;
static const int ROAD_PLACED = 1;

struct Game {
  int playerAmount;
  GameBoard board;
  RandomAI players[MAX_PLAYERS];
  int currentTurn;
  PlayerResources resources[MAX_PLAYERS];
  std::vector<std::vector<int>> history;
  bool currentPlayerRolled;
};

Game createGame(int);
void askAction(Game&);
void giveResourcesForVillage(Game&, int);
void roll(Game&);

#endif
