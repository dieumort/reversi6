/**
 * @file board.hpp
 */
#ifndef REVERSI6_BOARD_HPP_
#define REVERSI6_BOARD_HPP_

#include "common.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

/**
 * 6x6 bitboard
 */
class Board {
public:
  // constructor/destructor
  Board();
  Board(const Board & rhs) = default;
  Board(Board && rhs) = default;
  Board(int n, char const * moves[]);
  virtual ~Board() = default;

  // copy/move
  Board & operator=(const Board & rhs) = default;
  Board & operator=(Board && rhs) = default;

//-----------------------------------------------------------------------------

  // black score - white score
  int evaluate() const;

  // at least one legal move
  bool can_move() const;

  // one empty
  info_t get_move_if_one_empty() const;

  // legal moves pattern
  info_t generate_moves() const;

  // next mobility
  int get_next_mobility(info_t move) const;

  // set move
  void set_move(info_t move);

  // pass
  void pass();

//-----------------------------------------------------------------------------

private:
  // members
  info_t m_black;
  info_t m_white;

  // helper for generate_moves
  info_t generate_some_moves(int dir, info_t mask) const;

  // helper for set_moves
  info_t generate_some_flipped(info_t move, int dir, info_t mask) const;
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_BOARD_HPP_

//-----------------------------------------------------------------------------
