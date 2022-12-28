#include "game.hpp"
#include "terminal_ui.hpp"
#include "randomAI.hpp"
#include "action.hpp"
#include "data_structures.hpp"

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

  //displayBoardInfo(board);
  //std::cout << "\n\n";
  //displayBoard(board);

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

std::vector<int> getTilesNextToVillage(int village) {
  std::vector<int> neighbours;
  VillageAsTileXYConnection villageXY = villageIntToTileXYConnection(village);
  int deltas[6] = {1, -1, 0, -1, 0, 0};
  if (villageXY.isUpper) {
    for (int i = 0; i < 6; i++) {
      deltas[i] = - deltas[i];
    }
  }
  for (int i = 0; i < 3; i++) {
    int x = villageXY.x + deltas[i * 2] + MAP_RADIUS;
    int y = villageXY.y + deltas[i * 2 + 1] + MAP_RADIUS;
    if (x < 0 || y < 0 || x >= MAP_WIDTH || y >= MAP_WIDTH) {
      //std::cout << "Neighbour was not in region for which mapping works\n";
      continue;
    }
    int neighbour = y * MAP_WIDTH + x;
    if (!isValidTile(neighbour)) {
      continue;
    }
    neighbours.push_back(neighbour);
  }
  return neighbours;
}

void giveResourcesForVillage(Game& game, int village) {
  int player = game.board.villages[village];
  //std::cout << "Giving resources to player " << player << " for resources around village " << village << "\n";
  std::vector<int> tilesNextToVillage = getTilesNextToVillage(village);
  for (int i = 0; i < tilesNextToVillage.size(); i++) {
    int resource = game.board.tiles[tilesNextToVillage[i]];
    //std::cout << "Giving resource " << resource - 1 << " to player " << player << "\n";
    if (resource > 0) {
      game.resources[player].resources[resource - 1]++;
    }
    //std::cout << "One of it's neighbours is (" << arrs[i * 2] << ", " << arrs[i * 2 + 1] << ") -> " << neighbour << "\n";
  }
  return;
}

void moveRobberAndRob(Game& game) {
  // Move robber
  int abstractTileToMoveTo;
  do {
    abstractTileToMoveTo = game.players[game.currentTurn].moveRobber(game.board, game.resources);
  } while (abstractTileToMoveTo == game.board.robberLocation);
  //std::cout << "Player " << game.currentTurn << " moved robber from " << game.board.robberLocation << " to " << abstractTileToMoveTo << "\n";
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
      //std::cout << "Player " << game.currentTurn << " stole resource " << randomResource << " from player " << targetPlayer << "\n";
    }
  }
  // End robbing
}

void discardHalfResourcesFor7(Game& game) {
  for (int i = 0; i < game.playerAmount; i++) {
    int thisResourceAmount = 0;
    for (int j = 0; j < RESOURCE_TYPE_AMOUNT; j++) {
      thisResourceAmount += game.resources[i].resources[j];
    }
    //std::cout << "Player " << i << " has " << thisResourceAmount << " resources.\n";
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
    //std::cout << "After dicarding half resources, player " << i << " has " << thisResourceAmount << " resources.\n";
  }
}

void giveResourcesForRoll(Game& game, int rollTotal) {
  for (int i = 0; i < VILLAGE_ARRAY_LENGTH; i++) {
    if (game.board.villages[i] == -1) {
      continue;
    }
    int player = game.board.villages[i];
    std::vector<int> tiles = getTilesNextToVillage(i);

    for (int j = 0; j < tiles.size(); j++) {
      int neighbour = tiles[j];
      int number = numberAtTile(game.board, tileToAbstractTile(neighbour));
      if (number == rollTotal) {
        game.resources[player].resources[game.board.tiles[neighbour] - 1]++;
        if (game.board.cities[i] != -1) {
          game.resources[player].resources[game.board.tiles[neighbour] - 1]++;
          //std::cout << "Player " << player << " gets 2 of resource " << game.board.tiles[neighbour] - 1 << "\n";
        } else {
          //std::cout << "Player " << player << " gets resource " << game.board.tiles[neighbour] - 1 << "\n";
        }
      }
    }
  }
}

void roll(Game& game) {
  int rollTotal = rand() % 6 + rand() % 6 + 2;
  if (rollTotal == 7) {
    //std::cout << "Player " << game.currentTurn << " rolled 7\n";
    discardHalfResourcesFor7(game);
    moveRobberAndRob(game);
    return;
  }
  giveResourcesForRoll(game, rollTotal);
}

void checkWinner(Game& game) {
  for (int i = 0; i < game.playerAmount; i++) {
    if (game.playerPoints[i] >= 10) {
      game.winner = i;
    }
  }
}

void doVillagePlacement(Game& game) {
  int villageLocation;
  do {
    villageLocation = game.players[game.currentTurn].freeVillageLocation(game.board, game.currentTurn);
  }
  while (!isValidVillageLocation(game.board, villageLocation, game.currentTurn));
  //std::cout << "Player " << game.currentTurn << " placing village\n";
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
}

void initialPlacementTurnProgression(Game& game) {
  if (game.board.roadAmount <= game.playerAmount) {
    game.currentTurn++;
  }
  if (game.board.roadAmount >= game.playerAmount) {
    game.currentTurn--;
  }
  if (game.board.roadAmount == 2 * game.playerAmount) {
    game.currentTurn++;
  }
  game.currentTurn = game.currentTurn % game.playerAmount;
}

void doRoadPlacement(Game& game) {
  int roadLocation;
  int villageLocation = game.history.back()[2];
  do {
    roadLocation = game.players[game.currentTurn].freeRoadLocation(game.board, game.currentTurn, villageLocation);
  }
  while (!isValidRoadLocationNextToVillage(game.board, roadLocation, game.currentTurn, villageLocation));

  game.board.roads[roadLocation] = game.currentTurn;
  game.board.roadAmount++;
}

void doInitialPlacement(Game& game) {
  if (game.board.villageAmount == game.board.roadAmount) {
    doVillagePlacement(game);
  } else {
    doRoadPlacement(game);
    initialPlacementTurnProgression(game);
  }
}

bool doEndTurnAction(Game& game) {
  game.currentTurn++;
  game.currentTurn = game.currentTurn % game.playerAmount;
  game.currentPlayerRolled = false;
  return true;
}

bool doBuildRoadAction(Game& game, Action action) {
  if (isValidRoadLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
    //std::cout << "Player " << game.currentTurn << " built road\n";
    game.board.roads[action.actionLocation] = game.currentTurn;
    game.resources[game.currentTurn].resources[WOOD]--;
    game.resources[game.currentTurn].resources[CLAY]--;
    return true;
  }
  return false;
}

bool doTradeResourcesAction(Game& game, Action action) {
  if (game.resources[game.currentTurn].resources[action.resourceFrom] >= 4) {
    //std::cout << "Player " << game.currentTurn << " trades 4 " << RESOURCE_NAMES[action.resourceFrom] << " for 1 " << RESOURCE_NAMES[action.resourceTo] << "\n";
    game.resources[game.currentTurn].resources[action.resourceFrom] -= 4;
    game.resources[game.currentTurn].resources[action.resourceTo] += 1;
    return true;
  }
  return false;
}

bool doBuildVillageAction(Game& game, Action action) {
  if (isValidVillageLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
    //std::cout << "Player " << game.currentTurn << " builds a village at " << action.actionLocation << "\n";
    game.board.villages[action.actionLocation] = game.currentTurn;
    game.playerPoints[game.currentTurn]++;
    game.resources[game.currentTurn].resources[WOOD]--;
    game.resources[game.currentTurn].resources[CLAY]--;
    game.resources[game.currentTurn].resources[SHEEP]--;
    game.resources[game.currentTurn].resources[WHEAT]--;
    return true;
  }
  return false;
}

bool doBuildCityAction(Game& game, Action action) {
  if (isValidCityLocationForPlayer(game.board, action.actionLocation, game.currentTurn)) {
    //std::cout << "Player " << game.currentTurn << " builds a city at " << action.actionLocation << "\n";
    game.board.cities[action.actionLocation] = game.currentTurn;
    game.playerPoints[game.currentTurn]++;
    game.resources[game.currentTurn].resources[WHEAT] -= 2;
    game.resources[game.currentTurn].resources[ROCK] -= 3;
    return true;
  }
  return false;
}

bool doBuyDevelopmentCardAction(Game& game, Action action) {
  if (game.developmentCardsLeft > 0) {
    game.resources[game.currentTurn].resources[WHEAT]--;
    game.resources[game.currentTurn].resources[SHEEP]--;
    game.resources[game.currentTurn].resources[ROCK]--;
    int cardIndex = rand() % game.developmentCardsLeft;
    int card = game.developmentCardDeck[cardIndex];
    game.developmentCardsLeft--;
    game.developmentCardDeck[cardIndex] = game.developmentCardDeck[game.developmentCardsLeft];
    game.resources[game.currentTurn].developmentCards[card]++;
    //std::cout << "Player " << game.currentTurn << " bought development card " << card << "\n";
    if (card == VICTORY_POINT_CARD) {
      game.playerPoints[game.currentTurn]++;
    }
    return true;
  }
  return false;
}

bool tryToUseKnightCard(Game& game) {
  moveRobberAndRob(game);
  game.resources[game.currentTurn].developmentCards[KNIGHT_CARD]--;
  //std::cout << "Player " << game.currentTurn << " successfully played knight development card.\n";
  return true;
}

bool tryToUseYearOfPlentyCard(Game& game, Action action) {
  game.resources[game.currentTurn].developmentCards[YEAR_OF_PLENTY_CARD]--;
  game.resources[game.currentTurn].resources[action.secondaryActionLocation]++;
  game.resources[game.currentTurn].resources[action.tertiaryActionLocation]++;
  //std::cout << "Player " << game.currentTurn << " successfully played year of plenty development card and got resources " << action.secondaryActionLocation << " and " << action.tertiaryActionLocation << "\n";
  return true;
}

bool tryToUseMonopolyCard(Game& game, Action action) {
  game.resources[game.currentTurn].developmentCards[MONOPOLY_CARD]--;
  for (int i = 0; i < game.playerAmount; i++) {
    int resourceAmount = game.resources[i].resources[action.secondaryActionLocation];
    game.resources[i].resources[action.secondaryActionLocation] = 0;
    game.resources[game.currentTurn].resources[action.secondaryActionLocation] += resourceAmount;
  }
  //std::cout << "Player " << game.currentTurn << " successfully played monopoly development card and got all of resource " << action.secondaryActionLocation << "\n";
  return true;
}

bool tryToUseRoadBuildingCard(Game& game, Action action) {
  if (isValidRoadLocationForPlayer(game.board, action.secondaryActionLocation, game.currentTurn)) {
    game.board.roads[action.secondaryActionLocation] = game.currentTurn;
    if (isValidRoadLocationForPlayer(game.board, action.tertiaryActionLocation, game.currentTurn)) {
      game.board.roads[action.tertiaryActionLocation] = game.currentTurn;
      //std::cout << "Player " << game.currentTurn << " used road building successfully\n";
      return true;
    } else {
      game.board.roads[action.secondaryActionLocation] = -1;
      //std::cout << "Player " << game.currentTurn << " used road building unsuccessfully after building 1 road\n";
    }
  }
  return false;
}

bool doPlayDevelopmentCardAction(Game& game, Action action) {
  switch (action.actionLocation) {
    case KNIGHT_CARD: return tryToUseKnightCard(game);
    case YEAR_OF_PLENTY_CARD: return tryToUseYearOfPlentyCard(game, action);
    case MONOPOLY_CARD: return tryToUseMonopolyCard(game, action);
    case ROAD_BUILDING_CARD: return tryToUseRoadBuildingCard(game, action);
    default: return false;
  }
}

void doRollIfNeeded(Game& game) {
  if (!game.currentPlayerRolled) {
    roll(game);
    game.turnAmount++;
    game.currentPlayerRolled = true;
  }
}

void doNormalTurn(Game& game) {
  doRollIfNeeded(game);
  bool actionValid = false;
  while (!actionValid) {
    Action action = game.players[game.currentTurn].getAction(game.board, game.resources, game.developmentCardsLeft);
    switch (action.actionType) {
      case END_TURN: actionValid = doEndTurnAction(game); break;
      case BUILD_ROAD: actionValid = doBuildRoadAction(game, action); break;
      case TRADE_RESOURCES: actionValid = doTradeResourcesAction(game, action); break;
      case BUILD_VILLAGE: actionValid = doBuildVillageAction(game, action); break;
      case BUILD_CITY: actionValid = doBuildCityAction(game, action); break;
      case BUY_DEVELOPMENT_CARD: actionValid = doBuyDevelopmentCardAction(game, action); break;
      case PLAY_DEVELOPMENT_CARD: actionValid = doPlayDevelopmentCardAction(game, action); break;
    }
  }
}

void askAction(Game& game) {
  //std::cout << "Asking for action from player " << game.currentTurn << "\n";
  if (game.board.roadAmount < 2 * game.playerAmount) {
    doInitialPlacement(game);
  } else {
    doNormalTurn(game);
  }
  checkWinner(game);
  //std::cout << "Turn passed from player " << prevTurn << " to player " << game.currentTurn << "\n\n";
  return;
}
