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
  game.winner = -1;
  game.turnAmount = 0;
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
    game.playerPoints[i] = 0;
  }
  game.developmentCardsLeft = DEVELOPMENT_CARD_AMOUNT;
  int c = 0;
  for (int i = 0; i < DEVELOPMENT_CARD_TYPE_AMOUNT; i++) {
    for (int j = 0; j < DEVELOPMENT_CARD_AMOUNTS_BY_TYPE[i]; j++) {
      game.developmentCardDeck[c] = i;
      c++;
    }
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
  if (rollTotal == 7) {
    std::cout << "Player " << game.currentTurn << " rolled 7\n";
    for (int i = 0; i < game.playerAmount; i++) {
      int thisResourceAmount = 0;
      for (int j = 0; j < RESOURCE_TYPE_AMOUNT; j++) {
        thisResourceAmount += game.resources[i].resources[j];
      }
      std::cout << "Player " << i << " has " << thisResourceAmount << " resources.\n";
      // Discard resources
      if (thisResourceAmount > 7) {
        int amountDiscarded = 0;
        do {
          std::vector<int> discarded = game.players[i].discardResources(game.board, game.resources, thisResourceAmount / 2 - amountDiscarded);
          for (int j = 0; j < RESOURCE_TYPE_AMOUNT; j++) {
            game.resources[i].resources[j] -= discarded[j];
            amountDiscarded += discarded[j];
          }
        } while (amountDiscarded < thisResourceAmount / 2);
      }
      // End discard resources
      thisResourceAmount = 0;
      for (int j = 0; j < RESOURCE_TYPE_AMOUNT; j++) {
        thisResourceAmount += game.resources[i].resources[j];
      }
      std::cout << "After dicarding half resources, player " << i << " has " << thisResourceAmount << " resources.\n";
    }
    std::cout << "\n";
    // Move robber
    int abstractTileToMoveTo;
    do {
      abstractTileToMoveTo = game.players[game.currentTurn].moveRobber(game.board, game.resources);
    } while (abstractTileToMoveTo == game.board.robberLocation);
    std::cout << "Player " << game.currentTurn << " moved robber from " << game.board.robberLocation << " to " << abstractTileToMoveTo << "\n";
    game.board.robberLocation = abstractTileToMoveTo;
    // End move robber
    // Start robbing
    int tile = abstractTileToTile(game.board.robberLocation);
    std::vector<int> villages = villagesNextToTile(game.board, tile);
    if (villages.size() > 0) {
      int randomVillage = villages[rand() % villages.size()];
      int targetPlayer = game.board.villages[randomVillage];
      int randomResource = chooseRandomResource(game.resources[targetPlayer]);
      if (randomResource != -1) {
        game.resources[targetPlayer].resources[randomResource]--;
        game.resources[game.currentTurn].resources[randomResource]++;
        std::cout << "Player " << game.currentTurn << " stole resource " << randomResource << " from player " << targetPlayer << "\n";
      }
    }
    // End robbing
    return;
  }
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
        if (game.board.cities[i] != -1) {
          game.resources[player].resources[game.board.tiles[neighbour] - 1]++;
          std::cout << "Player " << player << " gets 2 of resource " << game.board.tiles[neighbour] - 1 << "\n";
        } else {
          std::cout << "Player " << player << " gets resource " << game.board.tiles[neighbour] - 1 << "\n";
        }
      }
    }
  }
}

void checkWinner(Game& game) {
  for (int i = 0; i < game.playerAmount; i++) {
    if (game.playerPoints[i] >= 10) {
      game.winner = i;
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
      std::cout << "Player " << game.currentTurn << " placing village\n";
      game.board.villages[villageLocation] = game.currentTurn;
      game.playerPoints[game.currentTurn]++;
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
      game.turnAmount++;
      game.currentPlayerRolled = true;
      if (game.currentTurn == 0) {
        for (int k = 0; k < 5; k++) {
          //game.resources[0].resources[k]++;
        }
      }
    }
    bool actionValid = false;
    while (!actionValid) {
      Action action = game.players[game.currentTurn].getAction(game.board, game.resources, game.developmentCardsLeft);
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
      } else if (action.actionType == BUILD_VILLAGE) {
        if (isValidVillageLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
          std::cout << "Player " << game.currentTurn << " builds a village at " << action.actionLocation << "\n";
          actionValid = true;
          game.board.villages[action.actionLocation] = game.currentTurn;
          game.playerPoints[game.currentTurn]++;
          game.resources[game.currentTurn].resources[WOOD]--;
          game.resources[game.currentTurn].resources[CLAY]--;
          game.resources[game.currentTurn].resources[SHEEP]--;
          game.resources[game.currentTurn].resources[WHEAT]--;
        }
      } else if (action.actionType == BUILD_CITY) {
        if (isValidCityLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
          std::cout << "Player " << game.currentTurn << " builds a city at " << action.actionLocation << "\n";
          actionValid = true;
          game.board.cities[action.actionLocation] = game.currentTurn;
          game.playerPoints[game.currentTurn]++;
          game.resources[game.currentTurn].resources[WHEAT] -= 2;
          game.resources[game.currentTurn].resources[ROCK] -= 3;
        }
      } else if (action.actionType == BUY_DEVELOPMENT_CARD) {
        if (game.developmentCardsLeft > 0) {
          actionValid = true;
          game.resources[game.currentTurn].resources[WHEAT]--;
          game.resources[game.currentTurn].resources[SHEEP]--;
          game.resources[game.currentTurn].resources[ROCK]--;
          int cardIndex = rand() % game.developmentCardsLeft;
          int card = game.developmentCardDeck[cardIndex];
          game.developmentCardsLeft--;
          game.developmentCardDeck[cardIndex] = game.developmentCardDeck[game.developmentCardsLeft];
          game.resources[game.currentTurn].developmentCards[card]++;
          std::cout << "Player " << game.currentTurn << " bought development card " << card << "\n";
        }
      } else {
        std::cout << "Warning, invalid action type " << action.actionType << "\n";
      }
    }
  }

  checkWinner(game);
  if (game.winner != -1) {
    std::cout << "Player " << game.winner << " has won, game ends!!!\n";
    return;
  }

  game.currentTurn = game.currentTurn % game.playerAmount;
  std::cout << "Turn passed from player " << prevTurn << " to player " << game.currentTurn << "\n\n";
  return;
}
