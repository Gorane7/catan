#include "game.hpp"
#include "terminal_ui.hpp"
#include "randomAI.hpp"

#include <iostream>

Game createGame(int playerAmount) {
  Game game;
  game.playerAmount = playerAmount;
  game.currentTurn = 0;
  GameBoard board = randomBoard(playerAmount);
  game.board = board;

  displayBoardInfo(board);
  std::cout << "\n\n";
  displayBoard(board);

  for (int i = 0; i < playerAmount; i++) {
    RandomAI ai;
    game.players[i] = ai;
    PlayerResources resources;
    game.resources[i] = resources;
  }
  return game;
}

void askAction(Game& game) {
  std::cout << "Asking for action from player " << game.currentTurn << "\n";
  int prevTurn = game.currentTurn;
  int preVillageAmount = game.board.villageAmount;
  int preRoadAmount = game.board.roadAmount;

  if (game.board.roadAmount < 2 * game.playerAmount) {
    // initial placement
    if (game.board.villageAmount == game.board.roadAmount) {
      // Village placement
      int villageLocation;
      do {
        villageLocation = game.players[game.currentTurn].freeVillageLocation(game.board, game.currentTurn);
      }
      while (!isValidVillageLocation(game.board, villageLocation, game.currentTurn));
      game.board.villages[villageLocation] = game.currentTurn;
      std::cout << "Player " << game.currentTurn << " placing village\n";
      game.board.villageAmount++;
      std::vector<int> thisAction;
      thisAction.push_back(VILLAGE_PLACED);
      thisAction.push_back(game.currentTurn);
      thisAction.push_back(villageLocation);
      game.history.push_back(thisAction);
    } else {
      // Road placement
      int roadLocation;
      int villageLocation = game.history.back()[2];
      do {
        roadLocation = game.players[game.currentTurn].freeRoadLocation(game.board, game.currentTurn, villageLocation);
      }
      while (!isValidRoadLocationNextToVillage(game.board, roadLocation, game.currentTurn, villageLocation));
      game.board.roads[roadLocation] = game.currentTurn;
      std::cout << "Player " << game.currentTurn << " placing village\n";
      game.board.roadAmount++;
      if (game.board.roadAmount <= game.playerAmount) {
        game.currentTurn++;
      }
      if (game.board.roadAmount >= game.playerAmount) {
        game.currentTurn--;
      }
      if (game.board.roadAmount == 2 * game.playerAmount) {
        game.currentTurn++;
      }
    }

  } else {
    // normal turn
    game.currentTurn++;
  }

  game.currentTurn = game.currentTurn % game.playerAmount;
  std::cout << "Turn passed from player " << prevTurn << " to player " << game.currentTurn << "\n\n";
  return;
}
