/**
 * @file board.hpp
 */
#ifndef REVERSI6_BOARD_HPP_
#define REVERSI6_BOARD_HPP_

#include <cstdint>
#include <vector>

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

/**
 * 6x6 bitboard
 */
class Board {
public:
  // constructor
  Board();
  Board(int n, char const * moves[]);

  // destructor
  virtual ~Board() = default;

  // copy
  Board & operator=(const Board & rhs) = default;
  Board(const Board & rhs) = default;

  // move
  Board(Board && rhs) = default;
  Board & operator=(Board && rhs) = default;

//-----------------------------------------------------------------------------

  // score (black - white)
  int evaluate() const;

  // has legals
  bool has_legals() const;

  // count empties
  int count_empties() const;

  // generate next Board vector
  std::vector<Board> generate_next_board_vector() const;

  // generate next Board vector (sorted)
  std::vector<Board> generate_next_board_sorted_vector() const;

  // play to end game if there is only one empty
  bool play_to_end_game_if_one_empty();

  // pass
  void pass();

//-----------------------------------------------------------------------------

private:
  // board state
  std::uint64_t m_black;
  std::uint64_t m_white;

  // candidates
  struct Candidates {
    std::uint64_t NS;
    std::uint64_t EW;
    std::uint64_t NESW;
    std::uint64_t NWSE;
  };

  // generate candidates
  Candidates generate_candidates() const;

  // count legals
  int count_legals() const;

  // generate legals
  std::uint64_t generate_legals() const;

  // generate legals with candidates
  std::uint64_t generate_legals(const Candidates & candidates) const;

  // helper for generate legals
  std::uint64_t generate_some_legals(int dir) const;

  // helper for generate legals with candidate
  std::uint64_t generate_some_legals(int dir, std::uint64_t candidate) const;

  // generate flipped
  std::uint64_t generate_flipped(std::uint64_t move) const;

  // generate flipped with candidates
  std::uint64_t generate_flipped(std::uint64_t move, const Candidates & candidates) const;

  // helper for generate flipped
  std::uint64_t generate_some_flipped(std::uint64_t move, int dir) const;

  // helper for generate flipped with candidate
  std::uint64_t generate_some_flipped(std::uint64_t move, int dir, std::uint64_t candidate) const;

  // helper for generate legals, flipped
  std::uint64_t generate_some_candidate(int dir) const;

  // play
  void play(std::uint64_t move);

  // play with rev
  void play(std::uint64_t move, std::uint64_t rev);

  // flip
  void flip(std::uint64_t move, std::uint64_t rev);
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_BOARD_HPP_

//-----------------------------------------------------------------------------
