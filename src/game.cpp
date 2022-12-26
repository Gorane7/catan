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

void giveResourcesForVillage(Game& game, int village) {
  int player = game.board.villages[village];
  std::cout << "Giving resources to player " << player << " for resources around village " << village << "\n";
  int tileX = villageArrToTileX(village);
  int tileY = villageArrToTileY(village);
  bool isUpper = villageArrToUpperBool(village);
  int arrs[6];
  if (isUpper) {
    arrs[0] = tileX - 1;
    arrs[1] = tileY + 1;
    arrs[2] = tileX;
    arrs[3] = tileY + 1;
    arrs[4] = tileX;
    arrs[5] = tileY;
  } else {
    arrs[0] = tileX + 1;
    arrs[1] = tileY - 1;
    arrs[2] = tileX;
    arrs[3] = tileY - 1;
    arrs[4] = tileX;
    arrs[5] = tileY;
  }
  for (int j = 0; j < MAP_WIDTH * MAP_WIDTH; j++) {
    std::cout << "Location " << j << " has resource " << game.board.tiles[j] << "\n";
  }
  for (int i = 0; i < 3; i++) {
    int x = arrs[i * 2] + MAP_RADIUS;
    int y = arrs[i * 2 + 1] + MAP_RADIUS;
    if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
      std::cout << "Neighbour was not in region for which mapping works\n";
      continue;
    }
    int neighbour = y * MAP_WIDTH + x;

    std::cout << "Neighbour was " << neighbour << "\n";

    if (!isValidTile(neighbour)) {
      continue;
    }

    std::cout << "Neighbour was valid\n";

    int resource = game.board.tiles[neighbour];
    std::cout << "Giving resource " << resource - 1 << " to player " << player << "\n";
    if (resource > 0) {
      game.resources[player].resources[resource - 1]++;
    }
    std::cout << "One of it's neighbours is (" << arrs[i * 2] << ", " << arrs[i * 2 + 1] << ") -> " << neighbour << "\n";
  }
  return;
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
      if (game.board.villageAmount > game.playerAmount) {
        giveResourcesForVillage(game, villageLocation);
      }
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
