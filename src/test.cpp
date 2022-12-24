#include "game.hpp"
#include "terminal_ui.hpp"
#include "actual_ui.hpp"

#include <iostream>

using namespace std;

int main(void){
  GameBoard board = randomBoard();
  displayBoardInfo(board);
  cout << "\n\n";
  displayBoard(board);
  run(board);
}
