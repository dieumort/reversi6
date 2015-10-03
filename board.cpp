/**
 * @file board.cpp
 */
#include <cctype>
#include <string>
#include <algorithm>
#include "board.hpp"

#include <iostream>

//-----------------------------------------------------------------------------

namespace {
// constants
const uint64_t MASK_BOARD = 0x007E7E7E7E7E7E00ULL;
const uint64_t INITIAL_BLACK = 0x0000000810000000ULL;
const uint64_t INITIAL_WHITE = 0x0000001008000000ULL;

const int NS = 8;
const int EW = 1;
const int NESW = 7;
const int NWSE = 9;

const uint64_t NS_MASK = 0x00007E7E7E7E0000ULL;
const uint64_t EW_MASK = 0x003C3C3C3C3C3C00ULL;
const uint64_t NESW_MASK = 0x00003C3C3C3C0000ULL;
const uint64_t NWSE_MASK = 0x00003C3C3C3C0000ULL;

const char * PASS_STR = "PASS";

// std::string to move
uint64_t to_move(char const * move_str) {
  const int col = move_str[0] - 'A';
  const int row = move_str[1] - '1';
  return 0x0040000000000000ULL >> (col + row * 8);
}

// count bits
int count_bits(uint64_t p) {
  p -= (p >> 1) & 0x5555555555555555ULL;
  p = ((p >> 2) & 0x3333333333333333ULL) + (p & 0x3333333333333333ULL);
  p = ((p >> 4) + p) & 0x0F0F0F0F0F0F0F0FULL;
  p += (p >> 8);
  p += (p >> 16);
  return ((p >> 32) + p) & 0x00000000000000FFULL;
}

} // namespace

//-----------------------------------------------------------------------------

// constructor/destructor
Board::Board()
: m_black(INITIAL_BLACK), m_white(INITIAL_WHITE) {
}

Board::Board(int n, char const * moves[])
: m_black(INITIAL_BLACK), m_white(INITIAL_WHITE) {
  // set moves
  for (int i = 0; i < n; ++i) {
    // to upper
    std::string move_str(moves[i]);
    std::transform(move_str.begin(), move_str.end(), move_str.begin(), ::toupper);

    // pass or not
    if (0 == move_str.compare(PASS_STR)) {
      pass();
    } else {
      const uint64_t move = to_move(move_str.c_str());
      set_move(move);
    }
  }
}

//-----------------------------------------------------------------------------

// no empty
bool Board::has_no_empty() const {
  return MASK_BOARD == (m_black | m_white);
}

// one empty
bool Board::has_one_empty() const {
  const uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;
  const uint64_t one_move = empties & (-empties);
  return empties == one_move;
}

// black score - white score
int Board::evaluate() const {
  return count_bits(m_black) - count_bits(m_white);
}

// at least one legal move
bool Board::can_move() const {
  return generate_some_moves(NS, NS_MASK) || generate_some_moves(EW, EW_MASK) || generate_some_moves(NESW, NESW_MASK) || generate_some_moves(NWSE, NWSE_MASK);
}

// legal moves pattern
uint64_t Board::generate_moves() const {
  uint64_t moves = generate_some_moves(NS, NS_MASK);
  moves |= generate_some_moves(EW, EW_MASK);
  moves |= generate_some_moves(NESW, NESW_MASK);
  moves |= generate_some_moves(NWSE, NWSE_MASK);
  return moves;
}

// set move
void Board::set_move(uint64_t move) {
  // generate flipped and set move
  uint64_t flipped = generate_some_flipped(move, NS, NS_MASK);
  flipped |= generate_some_flipped(move, EW, EW_MASK);
  flipped |= generate_some_flipped(move, NESW, NESW_MASK);
  flipped |= generate_some_flipped(move, NWSE, NWSE_MASK);

  // flip
  m_black ^= move | flipped;
  m_white ^= flipped;

  // change turn
  std::swap(m_black, m_white);
}

// pass
void Board::pass() {
  // change turn
  std::swap(m_black, m_white);
}

//-----------------------------------------------------------------------------

// helper for generate_moves
uint64_t Board::generate_some_moves(int dir, uint64_t mask) const {
  const uint64_t wh = m_white & mask;
  const uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;

  // flipped white for black
  uint64_t fb = (m_black << dir | m_black >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;

  // legal move = next && empty
  return (fb << dir | fb >> dir) & empties;
}

// helper for generate_flipped
uint64_t Board::generate_some_flipped(uint64_t move, int dir, uint64_t mask) const {
  const uint64_t wh = m_white & mask;

  // flipped white for black
  uint64_t fb = (m_black << dir | m_black >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;

  // flipped white for move
  uint64_t fm = (move << dir | move >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;

  // flipped white between black and move
  return fb & fm;
}

//-----------------------------------------------------------------------------
