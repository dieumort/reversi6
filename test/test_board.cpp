/**
 * @file test_board.cpp
 */
#include <iostream>
#include <boost/format.hpp>
#include "test_board.hpp"
#include "../board.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

// moves str
char const * moves_str[] = {"e4", "c5", "b4", "e3", "c2", "d2", "f3", "f4", "f5", "b3", "a3", "c1", "d5", "e5", "d1", "e1", "e2", "a5", "c6", "a4", "a6", "e6", "b5", "b6", "a2", "b2", "d6", "f6", "b1", "a1", "", "f2", "f1"};

// move pattens
constexpr info_t moves[] = {0x0000000004000000ULL, 0x0000000000100000ULL, 0x0000000020000000ULL, 0x0000000400000000ULL, 0x0000100000000000ULL, 0x0000080000000000ULL, 0x0000000200000000ULL, 0x0000000002000000ULL, 0x0000000000020000ULL, 0x0000002000000000ULL, 0x0000004000000000ULL, 0x0010000000000000ULL, 0x0000000000080000ULL, 0x0000000000040000ULL, 0x0008000000000000ULL, 0x0004000000000000ULL, 0x0000040000000000ULL, 0x0000000000400000ULL, 0x0000000000001000ULL, 0x0000000040000000ULL, 0x0000000000004000ULL, 0x0000000000000400ULL, 0x0000000000200000ULL, 0x0000000000002000ULL, 0x0000400000000000ULL, 0x0000200000000000ULL, 0x0000000000000800ULL, 0x0000000000000200ULL, 0x0020000000000000ULL, 0x0040000000000000ULL, 0x0000000000000000ULL, 0x0000020000000000ULL, 0x0002000000000000ULL};

// next moves pattern
constexpr info_t next_moves[] = {0x0000102004080000ULL, 0x0000000400140000ULL, 0x0000202020202000ULL, 0x0000004400440000ULL, 0x00001c0000001800ULL, 0x0010286042040000ULL, 0x0004040200041800ULL, 0x0010224042400000ULL, 0x000c0400000e1800ULL, 0x0010206040440000ULL, 0x0014644040001800ULL, 0x0010220040400000ULL, 0x002c000000282800ULL, 0x0000020040440800ULL, 0x002c000000203c00ULL, 0x0004460040400000ULL, 0x0000040000203c00ULL, 0x0000420040400000ULL, 0x0000000040203c00ULL, 0x0000220040202800ULL, 0x0020600000004c00ULL, 0x0000020000202c00ULL, 0x0020200000200800ULL, 0x0000020000002800ULL, 0x0020600000000800ULL, 0x0002220000000800ULL, 0x0020000000000800ULL, 0x0002020000000200ULL, 0x0020000000000000ULL, 0x0042020000000000ULL, 0x0000000000000000ULL, 0x0002020000000000ULL, 0x0002000000000000ULL, 0x0000000000000000ULL};

// evals
constexpr int evals[] = {0, -3, 0, -3, -2, -3, 0, -5, 0, -5, 0, -9, 2, -7, 2, -7, 2, -5, 2, -7, 2, -7, 2, -5, 0, -3, 0, -11, 6, -13, 8, -8, -7, 4};

//-----------------------------------------------------------------------------

// all
bool TestBoard::test_board() {
  std::cout << "test Board.init():";
  bool res = test_board_init();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.can_move():";
  res = test_board_can_move();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.evaluate():";
  res = test_board_evaluate();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.get_move_if_one_empty():";
  res = test_board_get_move_if_one_empty();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.generate_moves():";
  res = test_board_generate_moves();
  std::cout << (res ? "OK" : "NG") << std::endl;

  return true;
}

// init
bool TestBoard::test_board_init() {
 for (int i = 0; i < 33; ++i) {
   Board b(i, moves_str);

   // eval
   int e = b.evaluate();
   if (evals[i] != e) {
     std::cout << "NG (i=" << i << ")" << std::endl;
     std::cout << "ret=" << e << std::endl;
     std::cout << "ans=" << evals[i] << std::endl;
     return false;
   }

   // next moves
   info_t m = b.generate_moves();
   if (next_moves[i] != m) {
     std::cout << "NG (i=" << i << ")" << std::endl;
     std::cout << "ret=" << boost::format("%016x") % m << std::endl;
     std::cout << "ans=" << boost::format("%016x") % next_moves[i] << std::endl;
     return false;
   }
 }

 // ok
 return true;
}

 // can move
bool TestBoard::test_board_can_move() {
  Board b;

  // before black pass
  for (int i = 0; i < 30; ++i) {
    // check
    if (!b.can_move()) {
      std::cout << "NG (i=" << i << ")" << std::endl;
      return false;
    }

    // next
    b.set_move(moves[i]);
  }

  // check (can not move)
  if (b.can_move()) {
    std::cout << "NG (i=" << 30 << ")" << std::endl;
    return false;
  }

  // black pass
  b.pass();

  // check
  for (int i = 31; i < 33; ++i) {
    // check
    if (!b.can_move()) {
      std::cout << "NG (i=" << i << ")" << std::endl;
      return false;
    }

    // next
    b.set_move(moves[i]);
  }

  // check (no empty)
  if (b.can_move()) {
    std::cout << "NG (i=" << 33 << ")" << std::endl;
    return false;
  }

  // ok
  return true;
}

// black score - white score
bool TestBoard::test_board_evaluate() {
 Board b;
 for (int i = 0; i < 33; ++i) {
   // check
   int e = b.evaluate();
   if (evals[i] != e) {
     std::cout << "NG (i=" << i << ")" << std::endl;
     std::cout << "ret=" << e << std::endl;
     std::cout << "ans=" << evals[i] << std::endl;
     return false;
   }

   // next
   b.set_move(moves[i]);
 }

 // end game
 int e = b.evaluate();
 if (evals[33] != e) {
   std::cout << "NG (i=" << 33 << ")" << std::endl;
   std::cout << "ret=" << e << std::endl;
   std::cout << "ans=" << evals[33] << std::endl;
   return false;
 }

 // ok
 return true;
}

// one empty
bool TestBoard::test_board_get_move_if_one_empty() {
  Board b;

  // before one empty
  for (int i = 0; i < 32; ++i) {
    // check
    const info_t m = b.get_move_if_one_empty();
    if (m) {
      std::cout << "NG (i=" << i << ")" << std::endl;
      std::cout << "move=" << boost::format("%016x") % m << std::endl;
      return false;
    }

    // next
    b.set_move(moves[i]);
  }

  // check
  const info_t m32 = b.get_move_if_one_empty();
  if (0x0002000000000000ULL != m32) {
    std::cout << "NG (i=" << 32 << ")" << std::endl;
    std::cout << "move=" << boost::format("%016x") % m32 << std::endl;
    return false;
  }

  // next
  b.set_move(moves[32]);

  // check
  const info_t m33 = b.get_move_if_one_empty();
  if (m33) {
    std::cout << "NG (i=" << 33 << ")" << std::endl;
    std::cout << "move=" << boost::format("%016x") % m33 << std::endl;
    return false;
  }

  // ok
  return true;
}

// legal moves pattern
bool TestBoard::test_board_generate_moves() {
 Board b;
 for (int i = 0; i < 33; ++i) {
   // check
   info_t m = b.generate_moves();
   if (next_moves[i] != m) {
     std::cout << "NG (i=" << i << ")" << std::endl;
     std::cout << "ret=" << boost::format("%016x") % m << std::endl;
     std::cout << "ans=" << boost::format("%016x") % next_moves[i] << std::endl;
     return false;
   }

   // next
   b.set_move(moves[i]);
 }

 // end game
 info_t m = b.generate_moves();
 if (next_moves[33] != m) {
   std::cout << "NG (i=" << 33 << ")" << std::endl;
   std::cout << "ret=" << boost::format("%016x") % m << std::endl;
   std::cout << "ans=" << boost::format("%016x") % next_moves[33] << std::endl;
   return false;
 }

 // ok
 return true;
}

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------
