#include "game.hpp"
#include "terminal_ui.hpp"
#include "randomAI.hpp"
#include "action.hpp"

#include <iostream>

Game createGame(int playerAmount) {
  Game game;
  game.playerAmount = playerAmount;
  game.currentTurn = 0;
  game.currentPlayerRolled = false;
  GameBoard board = randomBoard(playerAmount);
  game.board = board;

  displayBoardInfo(board);
  std::cout << "\n\n";
  displayBoard(board);

  for (int i = 0; i < playerAmount; i++) {
    RandomAI ai;
    ai.index = i;
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
    //std::cout << "One of it's neighbours is (" << arrs[i * 2] << ", " << arrs[i * 2 + 1] << ") -> " << neighbour << "\n";
  }
  return;
}

void roll(Game& game) {
  int rollA = rand() % 6;
  int rollB = rand() % 6;
  std::cout << "Player " << game.currentTurn << " rolls " << rollA + 1 << " and " << rollB + 1 << "\n";
  int rollTotal = rollA + rollB + 2;
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (game.board.villages[i] == -1) {
      continue;
    }
    int player = game.board.villages[i];
    int tileX = villageArrToTileX(i);
    int tileY = villageArrToTileY(i);
    bool isUpper = villageArrToUpperBool(i);
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
    for (int j = 0; j < 3; j++) {
      int x = arrs[j * 2] + MAP_RADIUS;
      int y = arrs[j * 2 + 1] + MAP_RADIUS;
      if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
        continue;
      }
      int neighbour = y * MAP_WIDTH + x;
      if (!isValidTile(neighbour)) {
        continue;
      }
      //std::cout << "Tile (" << tileX << ", " << tileY << ") had neighbour (" << x << ", " << y << ")\n";
      //std::cout << "Tile " << neighbour << " in abstract counting is " << tileToAbstractTile(neighbour) << "\n";
      //std::cout << "Tile " << tileToAbstractTile(neighbour) << " has number " << numberAtTile(game.board, tileToAbstractTile(neighbour)) << "\n\n";
      int number = numberAtTile(game.board, tileToAbstractTile(neighbour));
      if (number == rollTotal) {
        game.resources[player].resources[game.board.tiles[neighbour] - 1]++;
        std::cout << "Player " << player << " gets resource " << game.board.tiles[neighbour] - 1 << "\n";
      }
    }
  }
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
    if (!game.currentPlayerRolled) {
      roll(game);
      game.currentPlayerRolled = true;
      if (game.currentTurn == 0) {
        for (int k = 0; k < 5; k++) {
          //game.resources[0].resources[k]++;
        }
      }
    }
    bool actionValid = false;
    while (!actionValid) {
      Action action = game.players[game.currentTurn].getAction(game.board, game.resources);
      if (action.actionType == END_TURN) {
        game.currentTurn++;
        actionValid = true;
        game.currentPlayerRolled = false;
      } else if (action.actionType == BUILD_ROAD) {
        if (isValidRoadLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
          std::cout << "Player " << game.currentTurn << " built road\n";
          actionValid = true;
          game.board.roads[action.actionLocation] = game.currentTurn;
          game.resources[game.currentTurn].resources[WOOD]--;
          game.resources[game.currentTurn].resources[CLAY]--;
        }
      } else if (action.actionType == TRADE_RESOURCES) {
        if (game.resources[game.currentTurn].resources[action.resourceFrom] >= 4) {
          std::cout << "Player " << game.currentTurn << " trades 4 " << RESOURCE_NAMES[action.resourceFrom] << " for 1 " << RESOURCE_NAMES[action.resourceTo] << "\n";
          actionValid = true;
          game.resources[game.currentTurn].resources[action.resourceFrom] -= 4;
          game.resources[game.currentTurn].resources[action.resourceTo] += 1;
        }
      } else {
        std::cout << "Warning, invalid action type " << action.actionType << "\n";
      }
    }

  }

  game.currentTurn = game.currentTurn % game.playerAmount;
  std::cout << "Turn passed from player " << prevTurn << " to player " << game.currentTurn << "\n\n";
  return;
}
