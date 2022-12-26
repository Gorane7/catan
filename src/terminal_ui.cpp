#include "terminal_ui.hpp"

#include <iostream>


void displayState(Game game) {
  for (int i = 0; i < game.playerAmount; i++) {
    for (int j = 0; j < RESOURCE_TYPE_AMOUNT; j++) {
      std::cout << "Player " << i << " has " << game.resources[i].resources[j] << " " << RESOURCE_NAMES[j] << "\n";
    }
    std::cout << "\n";
  }
  return;
}


void displayBoardInfo(GameBoard board) {
  std::cout << "Tiles on board: \n";
  int desertsFound = 0;
  for (int i = 0; i < TILE_AMOUNT; i++) {
    int tile = board.abstractTiles[i];

    std::string numberPart;
    if (tile == DESERT) {
      numberPart = "  ";
      desertsFound++;
    } else {
      int number = board.numbers[i - desertsFound];
      numberPart = (number < 10 ? " " : "") + std::to_string(number);
    }

    std::cout << "  (" << numberPart << ") "<< TILE_NAMES[tile] << (board.robberLocation == i ? " - ROBBER": "") << "\n";
  }
  std::cout << "\nNumbers on board: \n";
  for (int i = 0; i < NUMBER_AMOUNT; i++) {
    std::cout << "  " << board.numbers[i] << "\n";
  }
  std::cout << "The robber is at tile: " << board.robberLocation << "\n";
  return;
}

std::string getDisplayAtLocation(GameBoard board, int location) {
  int desertsFound = 0;
  if (board.robberLocation == location) {
    return " R ";
  }
  if (board.abstractTiles[location] == DESERT) {
    return "   ";
  }
  for (int i = 0; i < location; i++) {
    if (board.abstractTiles[i] == DESERT) {
      desertsFound++;
    }
  }
  int number = board.numbers[location - desertsFound];
  if (number < 10) {
    return " " + std::to_string(number) + " ";
  }
  return std::to_string(number) + " ";
}

void displayBoard(GameBoard board) {
  std::cout << "       ───── ───── ─────       \n";
  std::cout << "      |     |     |     |      \n";
  std::cout << "      | " << getDisplayAtLocation(board, 0) << " | " << getDisplayAtLocation(board, 1) << " | " << getDisplayAtLocation(board, 2) << " |      \n";
  std::cout << "      |     |     |     |      \n";
  std::cout << "    ───── ───── ───── ─────    \n";
  std::cout << "   |     |     |     |     |   \n";
  std::cout << "   | " << getDisplayAtLocation(board, 3) << " | " << getDisplayAtLocation(board, 4) << " | " << getDisplayAtLocation(board, 5) << " | " << getDisplayAtLocation(board, 6) << " |   \n";
  std::cout << "   |     |     |     |     |   \n";
  std::cout << " ───── ───── ───── ───── ───── \n";
  std::cout << "|     |     |     |     |     |\n";
  std::cout << "| " << getDisplayAtLocation(board, 7) << " | " << getDisplayAtLocation(board, 8) << " | " << getDisplayAtLocation(board, 9) << " | " << getDisplayAtLocation(board, 10) << " | " << getDisplayAtLocation(board, 11) << " |\n";
  std::cout << "|     |     |     |     |     |\n";
  std::cout << " ───── ───── ───── ───── ───── \n";
  std::cout << "   |     |     |     |     |   \n";
  std::cout << "   | " << getDisplayAtLocation(board, 12) << " | " << getDisplayAtLocation(board, 13) << " | " << getDisplayAtLocation(board, 14) << " | " << getDisplayAtLocation(board, 15) << " |   \n";
  std::cout << "   |     |     |     |     |   \n";
  std::cout << "    ───── ───── ───── ─────    \n";
  std::cout << "      |     |     |     |      \n";
  std::cout << "      | " << getDisplayAtLocation(board, 16) << " | " << getDisplayAtLocation(board, 17) << " | " << getDisplayAtLocation(board, 18) << " |      \n";
  std::cout << "      |     |     |     |      \n";
  std::cout << "       ───── ───── ─────       \n";
  return;
}
