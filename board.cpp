/**
 * @file board.cpp
 */
#include <cctype>
#include <cstring>
#include <utility>
#include "board.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

constexpr info_t MASK_BOARD = 0x007E7E7E7E7E7E00ULL;
constexpr info_t INITIAL_BLACK = 0x0000000810000000ULL;
constexpr info_t INITIAL_WHITE = 0x0000001008000000ULL;
constexpr info_t UPPER_LEFT_MOVE = 0x0040000000000000ULL;

constexpr int NS = 8;
constexpr int EW = 1;
constexpr int NESW = 7;
constexpr int NWSE = 9;

constexpr info_t NS_MASK = 0x00007E7E7E7E0000ULL;
constexpr info_t EW_MASK = 0x003C3C3C3C3C3C00ULL;
constexpr info_t NESW_MASK = 0x00003C3C3C3C0000ULL;
constexpr info_t NWSE_MASK = 0x00003C3C3C3C0000ULL;

int count_bits(std::uint64_t p) {
  p -= (p >> 1) & 0x5555555555555555ULL;
  p = ((p >> 2) & 0x3333333333333333ULL) + (p & 0x3333333333333333ULL);
  p = ((p >> 4) + p) & 0x0F0F0F0F0F0F0F0FULL;
  p += (p >> 8);
  p += (p >> 16);
  return ((p >> 32) + p) & 0x00000000000000FFULL;
}
//-----------------------------------------------------------------------------

// constructor/destructor
Board::Board()
: m_black(INITIAL_BLACK), m_white(INITIAL_WHITE) {
}

Board::Board(int n, char const * moves[])
: m_black(INITIAL_BLACK), m_white(INITIAL_WHITE) {
  // set moves
  for (int i = 0; i < n; ++i) {
    const char * move_str = moves[i];
    // length must be 2
    if (2 != std::strlen(move_str)) {
      pass();
      continue;
    }

    // get coordinate
    const int col = std::toupper(move_str[0]) - 'A';
    const int row = move_str[1] - '1';

    // 0 <= col <=7, 0 <= row <= 7
    if (col < 0 || 7 < col || row < 0 || 7 < row) {
      pass();
      continue;
    }

    const info_t move = UPPER_LEFT_MOVE >> (col + row * 8);
    set_move(move);
  }
}

//-----------------------------------------------------------------------------

// black score - white score
int Board::evaluate() const {
  return count_bits(m_black) - count_bits(m_white);
}

// at least one legal move
bool Board::can_move() const {
  return generate_some_moves(NS, NS_MASK) || generate_some_moves(EW, EW_MASK) || generate_some_moves(NESW, NESW_MASK) || generate_some_moves(NWSE, NWSE_MASK);
}

// one empty
info_t Board::get_move_if_one_empty() const {
  const info_t empties = (~(m_black | m_white)) & MASK_BOARD;
  const info_t one_move = empties & (-empties);
  const info_t mask = - ((((empties ^ one_move) >> 1) - 1) >> 63);
  return one_move & mask;
}

// legal moves pattern
info_t Board::generate_moves() const {
  info_t moves = generate_some_moves(NS, NS_MASK);
  moves |= generate_some_moves(EW, EW_MASK);
  moves |= generate_some_moves(NESW, NESW_MASK);
  moves |= generate_some_moves(NWSE, NWSE_MASK);
  return moves;
}

// next mobility
int Board::get_next_mobility(info_t move) const {
  Board next(*this);
  next.set_move(move);

  return count_bits(next.generate_moves());
}

// set move
void Board::set_move(info_t move) {
  // generate flipped
  info_t flipped = generate_some_flipped(move, NS, NS_MASK);
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
info_t Board::generate_some_moves(int dir, info_t mask) const {
  const info_t wh = m_white & mask;
  const info_t empties = (~(m_black | m_white)) & MASK_BOARD;

  // flipped white for black
  info_t fb = (m_black << dir | m_black >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;

  // legal move = next && empty
  return (fb << dir | fb >> dir) & empties;
}

// helper for generate_flipped
info_t Board::generate_some_flipped(info_t move, int dir, info_t mask) const {
  const info_t wh = m_white & mask;

  // flipped white for black
  info_t fb = (m_black << dir | m_black >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;

  // flipped white for move
  info_t fm = (move << dir | move >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;

  // flipped white between black and move
  return fb & fm;
}

//-----------------------------------------------------------------------------

} // reversi6

//-----------------------------------------------------------------------------
