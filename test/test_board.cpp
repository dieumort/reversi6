/**
 * @file test_board.cpp
 */
#include <iostream>
#include <boost/format.hpp>
#include "test_board.hpp"
#include "../common.hpp"
#include "../board.hpp"

//-----------------------------------------------------------------------------

namespace {
  // moves str
  char const * moves_str[] = {"e4", "c5", "b4", "e3", "c2", "d2", "f3", "f4", "f5", "b3", "a3", "c1", "d5", "e5", "d1", "e1", "e2", "a5", "c6", "a4", "a6", "e6", "b5", "b6", "a2", "b2", "d6", "f6", "b1", "a1", "pass", "f2", "f1"};

  // move pattens
  uint64_t moves[] = {0x0000000004000000, 0x0000000000100000, 0x0000000020000000, 0x0000000400000000, 0x0000100000000000, 0x0000080000000000, 0x0000000200000000, 0x0000000002000000, 0x0000000000020000, 0x0000002000000000, 0x0000004000000000, 0x0010000000000000, 0x0000000000080000, 0x0000000000040000, 0x0008000000000000, 0x0004000000000000, 0x0000040000000000, 0x0000000000400000, 0x0000000000001000, 0x0000000040000000, 0x0000000000004000, 0x0000000000000400, 0x0000000000200000, 0x0000000000002000, 0x0000400000000000, 0x0000200000000000, 0x0000000000000800, 0x0000000000000200, 0x0020000000000000, 0x0040000000000000, 0x0000000000000000, 0x0000020000000000, 0x0002000000000000};

  // next moves pattern
  uint64_t next_moves[] = {0x0000102004080000, 0x0000000400140000, 0x0000202020202000, 0x0000004400440000, 0x00001c0000001800, 0x0010286042040000, 0x0004040200041800, 0x0010224042400000, 0x000c0400000e1800, 0x0010206040440000, 0x0014644040001800, 0x0010220040400000, 0x002c000000282800, 0x0000020040440800, 0x002c000000203c00, 0x0004460040400000, 0x0000040000203c00, 0x0000420040400000, 0x0000000040203c00, 0x0000220040202800, 0x0020600000004c00, 0x0000020000202c00, 0x0020200000200800, 0x0000020000002800, 0x0020600000000800, 0x0002220000000800, 0x0020000000000800, 0x0002020000000200, 0x0020000000000000, 0x0042020000000000, 0x0000000000000000, 0x0002020000000000, 0x0002000000000000, 0x0000000000000000};

  // evals
  int evals[] = {0, -3, 0, -3, -2, -3, 0, -5, 0, -5, 0, -9, 2, -7, 2, -7, 2, -5, 2, -7, 2, -7, 2, -5, 0, -3, 0, -11, 6, -13, 8, -8, -7, 4};

} // namespace

//-----------------------------------------------------------------------------

namespace test {

// all
bool test_board() {
  std::cout << "test Board.init():";
  bool res = test_board_init();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board status:";
  res = test_board_status();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.evaluate():";
  res = test_board_evaluate();
  std::cout << (res ? "OK" : "NG") << std::endl;
  std::cout << "test Board.generate_moves():";
  res = test_board_generate_moves();
  std::cout << (res ? "OK" : "NG") << std::endl;

  return true;
}

// init
bool test_board_init() {
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
   uint64_t m = b.generate_moves();
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

 // status
bool test_board_status() {
  Board b;

  // before black pass
  for (int i = 0; i < 30; ++i) {
    // check
    if (b.has_no_empty() || b.has_one_empty() || !b.can_move()) {
      std::cout << "NG (i=" << i << ")" << std::endl;
      std::cout << "no_empty:" << std::boolalpha << b.has_no_empty() << std::endl;
      std::cout << "one_empty:" << std::boolalpha << b.has_one_empty() << std::endl;
      std::cout << "can_move:" << std::boolalpha << b.can_move() << std::endl;
      return false;
    }

    // next
    b.set_move(moves[i]);
  }

  // check (can not move)
  if (b.has_no_empty() || b.has_one_empty() || b.can_move()) {
    std::cout << "NG (i=" << 30 << ")" << std::endl;
    std::cout << "no_empty:" << std::boolalpha << b.has_no_empty() << std::endl;
    std::cout << "one_empty:" << std::boolalpha << b.has_one_empty() << std::endl;
    std::cout << "can_move:" << std::boolalpha << b.can_move() << std::endl;
    return false;
  }

  // black pass
  b.pass();

  // check
  if (b.has_no_empty() || b.has_one_empty() || !b.can_move()) {
    std::cout << "NG (i=" << 31 << ")" << std::endl;
    std::cout << "no_empty:" << std::boolalpha << b.has_no_empty() << std::endl;
    std::cout << "one_empty:" << std::boolalpha << b.has_one_empty() << std::endl;
    std::cout << "can_move:" << std::boolalpha << b.can_move() << std::endl;
    return false;
  }

  // white last move
  b.set_move(moves[31]);

  // check (one empty)
  if (b.has_no_empty() || !b.has_one_empty() || !b.can_move()) {
    std::cout << "NG (i=" << 32 << ")" << std::endl;
    std::cout << "no_empty:" << std::boolalpha << b.has_no_empty() << std::endl;
    std::cout << "one_empty:" << std::boolalpha << b.has_one_empty() << std::endl;
    std::cout << "can_move:" << std::boolalpha << b.can_move() << std::endl;
    return false;
  }

  // black last move
  b.set_move(moves[32]);

  // check (no empty)
  if (!b.has_no_empty() || !b.has_one_empty() || b.can_move()) {
    std::cout << "NG (i=" << 33 << ")" << std::endl;
    std::cout << "no_empty:" << std::boolalpha << b.has_no_empty() << std::endl;
    std::cout << "one_empty:" << std::boolalpha << b.has_one_empty() << std::endl;
    std::cout << "can_move:" << std::boolalpha << b.can_move() << std::endl;
    return false;
  }

  // ok
  return true;
}

// black score - white score
bool test_board_evaluate() {
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

// legal moves pattern
bool test_board_generate_moves() {
 Board b;
 for (int i = 0; i < 33; ++i) {
   // check
   uint64_t m = b.generate_moves();
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
 uint64_t m = b.generate_moves();
 if (next_moves[33] != m) {
   std::cout << "NG (i=" << 33 << ")" << std::endl;
   std::cout << "ret=" << boost::format("%016x") % m << std::endl;
   std::cout << "ans=" << boost::format("%016x") % next_moves[33] << std::endl;
   return false;
 }

 // ok
 return true;
}

} // namespace test

//-----------------------------------------------------------------------------
