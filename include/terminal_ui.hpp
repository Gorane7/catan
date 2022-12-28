#ifndef TERMINAL_UI_HPP
#define TERMINAL_UI_HPP

#include "game_board.hpp"
#include "game.hpp"

const std::string RESOURCE_NAMES[RESOURCE_TYPE_AMOUNT] = {"Wood", "Sheep", "Wheat", "Rock", "Clay"};

const std::string DEVELOPMENT_CARD_NAMES[DEVELOPMENT_CARD_TYPE_AMOUNT] = {"Knight", "Victory point", "Road building", "Year of plenty", "Monopoly"};

void displayBoardInfo(GameBoard);
void displayBoard(GameBoard);
void displayState(Game);

#endif
