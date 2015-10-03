/**
 * @file board.hpp
 */
#ifndef BOARD_HPP_
#define BOARD_HPP_

#include <string>
#include "common.hpp"

//-----------------------------------------------------------------------------

/**
 * 6x6 bitboard
 */
class Board {
public:
  // constructor/destructor
  Board();
  Board(const Board & board) = default;
  Board(Board && board) = default;
  Board(int n, char const * moves[]);
  virtual ~Board() = default;

  // copy/move
  Board & operator=(const Board & lhs) = default;
  Board & operator=(Board && lhs) = default;

//-----------------------------------------------------------------------------

  // no empty
  bool has_no_empty() const;

  // one empty
  bool has_one_empty() const;

  // black score - white score
  int evaluate() const;

  // at least one legal move
  bool can_move() const;

  // legal moves pattern
  uint64_t generate_moves() const;

  // set move
  void set_move(uint64_t move);

  // pass
  void pass();

//-----------------------------------------------------------------------------

private:
  // members
  uint64_t m_black;
  uint64_t m_white;

  // helper for generate_moves
  uint64_t generate_some_moves(int dir, uint64_t mask) const;

  // helper for set_moves
  uint64_t generate_some_flipped(uint64_t move, int dir, uint64_t mask) const;
};

//-----------------------------------------------------------------------------

#endif // BOARD_HPP_

//-----------------------------------------------------------------------------
