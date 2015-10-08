/**
 * @file test_board.hpp
 */
#ifndef REVERSI6_TEST_BOARD_HPP_
#define REVERSI6_TEST_BOARD_HPP_

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

class TestBoard {
public:
  // constructor/destructor
  TestBoard() = default;
  TestBoard(const TestBoard & rhs) = default;
  TestBoard(TestBoard && rhs) = default;
  virtual ~TestBoard() = default;

  // copy/move
  TestBoard & operator=(const TestBoard & rhs) = default;
  TestBoard & operator=(TestBoard && rhs) = default;

//-----------------------------------------------------------------------------

  // all
  bool test_board();

  // init
  bool test_board_init();

  // can move
  bool test_board_can_move();

  // black score - white score
  bool test_board_evaluate();

  // one empty
  bool test_board_get_move_if_one_empty();

  // legal moves pattern
  bool test_board_generate_moves();
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_TEST_BOARD_HPP_

//-----------------------------------------------------------------------------
