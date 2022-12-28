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

static const int DEVELOPMENT_CARD_AMOUNTS_BY_TYPE[DEVELOPMENT_CARD_TYPE_AMOUNT] = {14, 5, 2, 2, 2};
static const int DEVELOPMENT_CARD_AMOUNT = 25;

struct Game {
  int playerAmount;
  GameBoard board;
  RandomAI players[MAX_PLAYERS];
  int currentTurn;
  PlayerResources resources[MAX_PLAYERS];
  std::vector<std::vector<int>> history;
  bool currentPlayerRolled;
  int playerPoints[MAX_PLAYERS];
  int winner;
  int turnAmount;
  int developmentCardDeck[DEVELOPMENT_CARD_AMOUNT];
  int developmentCardsLeft;
};

Game createGame(int);
void askAction(Game&);
void giveResourcesForVillage(Game&, int);
void roll(Game&);

#endif
