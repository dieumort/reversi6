/**
 * @file test_board.hpp
 */
#ifndef TEST_TEST_BOARD_HPP_
#define TEST_TEST_BOARD_HPP_

//-----------------------------------------------------------------------------

namespace test {

// all
bool test_board();

// init
bool test_board_init();

// status
bool test_board_status();

// black score - white score
bool test_board_evaluate();

// legal moves pattern
bool test_board_generate_moves();

} // namespace test

//-----------------------------------------------------------------------------

#endif // TEST_TEST_BOARD_HPP_

//-----------------------------------------------------------------------------
