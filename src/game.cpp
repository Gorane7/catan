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
  }
  return game;
}

void askAction(Game& game) {
  std::cout << "Asking for action from player " << game.currentTurn << "\n";
  int prevTurn = game.currentTurn;
  int preVillageAmount = game.board.villageAmount;

  if (preVillageAmount < 2 * game.playerAmount) {
    int villageLocation;
    do {
      villageLocation = game.players[game.currentTurn].freeVillageLocation(game.board, game.currentTurn);
    }
    while (!isValidVillageLocation(game.board, villageLocation, game.currentTurn));
    game.board.villages[villageLocation] = game.currentTurn;
    //std::cout << "Player " << game.currentTurn << " placing village\n";
    game.board.villageAmount++;
  }
  if (preVillageAmount < game.playerAmount - 1) {
    game.currentTurn++;
  } else if (preVillageAmount == game.playerAmount - 1) {
  } else if (preVillageAmount < 2 * game.playerAmount - 1) {
    game.currentTurn--;
  } else if (preVillageAmount == 2 * game.playerAmount - 1) {
  } else {
    game.currentTurn++;
  }

  game.currentTurn = game.currentTurn % game.playerAmount;
  std::cout << "Turn passed from player " << prevTurn << " to player " << game.currentTurn << "\n\n";
  return;
}
