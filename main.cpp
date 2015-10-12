/**
 * main.cpp
 */
#include <iostream>
#include "board.hpp"
#include "pvs.hpp"

int main(int argc, char const * argv[]) {
  using namespace std;
  using namespace reversi6;

  Board board(argc - 1, argv + 1);
  Pvs pvs;
  std::cout << pvs.search(board) << std::endl;

  return 0;
}
