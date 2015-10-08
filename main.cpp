/**
 * main.cpp
 */
#include <iostream>
#include "board.hpp"
#include "pvs.hpp"
#include "test/test_board.hpp"

int main(int argc, char const * argv[]) {
  using namespace std;
  using namespace reversi6;

  //TestBoard test;
  //test.test_board();

  Board b(argc - 1, argv + 1);
  Pvs pvs;
  std::cout << pvs.search(b) << std::endl;

  return 0;
}
