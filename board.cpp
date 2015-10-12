/**
 * @file board.cpp
 */
#include <cctype>
#include <cstring>
#include <array>
#include <utility>
#include <algorithm>
#include "board.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

constexpr std::uint64_t MASK_BOARD = 0x007E7E7E7E7E7E00ULL;
constexpr std::uint64_t INITIAL_BLACK = 0x0000000810000000ULL;
constexpr std::uint64_t INITIAL_WHITE = 0x0000001008000000ULL;
constexpr std::uint64_t UPPER_LEFT_MOVE = 0x0040000000000000ULL;

constexpr int NS = 8;
constexpr int EW = 1;
constexpr int NESW = 7;
constexpr int NWSE = 9;

// masks
constexpr std::uint64_t MASKS[] = {0, 0x003C3C3C3C3C3C00ULL, 0, 0, 0, 0, 0, 0x00003C3C3C3C0000ULL, 0x00007E7E7E7E0000ULL, 0x00003C3C3C3C0000ULL};

// static value
// 32 32 32 32 32 32 32 32
// 32 03 25 09 07 23 01 32
// 32 27 31 17 15 29 21 32
// 32 11 19 32 32 13 05 32
// 32 10 18 32 32 12 04 32
// 32 26 30 16 14 28 20 32
// 32 02 24 08 06 22 00 32
// 32 32 32 32 32 32 32 32
constexpr int STATIC_VALUES[] = {32, 32, 32, 32, 32, 32, 32, 32, 32, 0, 22, 6, 8, 24, 2, 32, 32, 20, 28, 14, 16, 30, 26, 32, 32, 4, 12, 32, 32, 18, 10, 32, 32, 5, 13, 32, 32, 19, 11, 32, 32, 21, 29, 15, 17, 31, 27, 32, 32, 1, 23, 7, 9, 25, 3, 32, 32, 32, 32, 32, 32, 32, 32, 32};

int count_bits(std::uint64_t p) {
  p -= (p >> 1) & 0x5555555555555555ULL;
  p = ((p >> 2) & 0x3333333333333333ULL) + (p & 0x3333333333333333ULL);
  p = ((p >> 4) + p) & 0x0F0F0F0F0F0F0F0FULL;
  p += (p >> 8);
  p += (p >> 16);
  return ((p >> 32) + p) & 0x00000000000000FFULL;
}
//-----------------------------------------------------------------------------

// constructor
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

    // play move
    const uint64_t move = UPPER_LEFT_MOVE >> (col + row * 8);
    play(move);
  }
}

//-----------------------------------------------------------------------------

// score (black - white)
int Board::evaluate() const {
  return count_bits(m_black) - count_bits(m_white);
}

// has legals
bool Board::has_legals() const {
  return generate_some_legals(NS) || generate_some_legals(EW) || generate_some_legals(NESW) || generate_some_legals(NWSE);
}

// count empties
int Board::count_empties() const {
  const uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;
  return count_bits(empties);
}

// generate next Board vector
std::vector<Board> Board::generate_next_board_vector() const {
  // candidates
  Candidates candidates(generate_candidates());

  // legals
  std::uint64_t legals = generate_legals(candidates);
  const int legals_size = count_bits(legals);

  // next vector
  std::vector<Board> vec;
  vec.reserve(legals_size);

  while (legals) {
    // legals
    const std::uint64_t legal = legals & (-legals);
    const std::uint64_t rev = generate_flipped(legal, candidates);

    // next
    Board next(*this);
    next.play(legal, rev);
    vec.emplace_back(std::move(next));

    legals ^= legal;
  }

  return vec;
}

// generate next Board vector (sorted)
std::vector<Board> Board::generate_next_board_sorted_vector() const {
  // candidates
  Candidates candidates(generate_candidates());

  // legals
  std::uint64_t legals = generate_legals(candidates);
  const int legals_size = count_bits(legals);

  // next vector
  std::vector<Board> vec;
  vec.reserve(legals_size);

  // score vector
  std::vector<int> score_vec;
  score_vec.reserve(legals_size);

  while (legals) {
    // legals
    const std::uint64_t legal = legals & (-legals);
    const std::uint64_t rev = generate_flipped(legal, candidates);

    // next
    Board next(*this);
    next.play(legal, rev);

    // STATIC_VALUES[NTZ] & mobility
    int score = STATIC_VALUES[count_bits(legal - 1)];
    score += next.count_legals() << 8;

    // add
    vec.emplace_back(std::move(next));
    score_vec.push_back(score);

    // sort
    for (int i = vec.size() - 1; i != 0; --i) {
      if (score_vec[i] < score_vec[i - 1]) {
        std::swap(vec[i], vec[i - 1]);
        std::swap(score_vec[i], score_vec[i - 1]);
      }
    }

    legals ^= legal;
  }

  return vec;
}

// play to end game if there is only one empty
bool Board::play_to_end_game_if_one_empty() {
  // get one move
  const uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;
  const uint64_t one_move = empties & (-empties);

  // there are two or more moves
  if (empties != one_move) {
    return false;
  }

  // play move
  const uint64_t rev = generate_flipped(one_move);
  if (rev) {
    // flip
    flip(one_move, rev);
  } else {
    // change turn & flip & change turn
    pass();
    const uint64_t rev_white = generate_flipped(one_move);
    if (rev_white) {
      flip(one_move, rev_white);
    }
    pass();
  }

  // black turn
  return true;
}

// pass
void Board::pass() {
  // change turn
  std::swap(m_black, m_white);
}

//-----------------------------------------------------------------------------

// generate candidates
Board::Candidates Board::generate_candidates() const {
  Candidates candidates;
  candidates.NS = generate_some_candidate(NS);
  candidates.EW = generate_some_candidate(EW);
  candidates.NESW = generate_some_candidate(NESW);
  candidates.NWSE = generate_some_candidate(NWSE);
  return candidates;
}

// count legals
int Board::count_legals() const {
  return count_bits(generate_legals());
}

// generate legals
std::uint64_t Board::generate_legals() const {
  std::uint64_t legals = generate_some_legals(NS);
  legals |= generate_some_legals(EW);
  legals |= generate_some_legals(NESW);
  legals |= generate_some_legals(NWSE);
  return legals;
}

// generate legals with candidates
std::uint64_t Board::generate_legals(const Candidates & candidates) const {
  std::uint64_t legals = generate_some_legals(NS, candidates.NS);
  legals |= generate_some_legals(EW, candidates.EW);
  legals |= generate_some_legals(NESW, candidates.NESW);
  legals |= generate_some_legals(NWSE, candidates.NWSE);
  return legals;
}

// helper for generate legals
std::uint64_t Board::generate_some_legals(int dir) const {
  // empties
  const std::uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;

  // flipped white for black
  std::uint64_t fb = generate_some_candidate(dir);

  // legal move = next && empty
  return (fb << dir | fb >> dir) & empties;
}

// helper for generate legals with candidate
std::uint64_t Board::generate_some_legals(int dir, std::uint64_t candidate) const {
  // empties
  const std::uint64_t empties = (~(m_black | m_white)) & MASK_BOARD;

  // legal move = next && empty
  return (candidate << dir | candidate >> dir) & empties;
}

// generate flipped
std::uint64_t Board::generate_flipped(std::uint64_t move) const {
  std::uint64_t rev = generate_some_flipped(move, NS);
  rev |= generate_some_flipped(move, EW);
  rev |= generate_some_flipped(move, NESW);
  rev |= generate_some_flipped(move, NWSE);
  return rev;
}

// generate flipped with candidates
std::uint64_t Board::generate_flipped(std::uint64_t move, const Candidates & candidates) const {
  std::uint64_t rev = generate_some_flipped(move, NS, candidates.NS);
  rev |= generate_some_flipped(move, EW, candidates.EW);
  rev |= generate_some_flipped(move, NESW, candidates.NESW);
  rev |= generate_some_flipped(move, NWSE, candidates.NWSE);
  return rev;
}

// helper for generate_flipped
std::uint64_t Board::generate_some_flipped(std::uint64_t move, int dir) const {
  // masked white
  const std::uint64_t wh = m_white & MASKS[dir];

  // flipped white for black
  const std::uint64_t fb = generate_some_candidate(dir);

  // flipped white for move
  std::uint64_t fm = (move << dir | move >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;

  // flipped white between black and move
  return fb & fm;
}

// helper for generate flipped with candidate
std::uint64_t Board::generate_some_flipped(std::uint64_t move, int dir, std::uint64_t candidate) const {
  // masked white
  const std::uint64_t wh = m_white & MASKS[dir];

  // flipped white for move
  std::uint64_t fm = (move << dir | move >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;
  fm |= (fm << dir | fm >> dir) & wh;

  // flipped white between black and move
  return candidate & fm;
}

// helper for generate legals, flipped
std::uint64_t Board::generate_some_candidate(int dir) const {
  // masked white
  const std::uint64_t wh = m_white & MASKS[dir];

  // flipped white for black
  std::uint64_t fb = (m_black << dir | m_black >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;
  fb |= (fb << dir | fb >> dir) & wh;

  return fb;
}

// play
void Board::play(std::uint64_t move) {
  // generate flipped
  const std::uint64_t rev = generate_flipped(move);

  // flip
  flip(move, rev);

  // change turn
  pass();
}

// play with rev
void Board::play(std::uint64_t move, std::uint64_t rev) {
  // flip
  flip(move, rev);

  // change turn
  pass();
}

// flip
void Board::flip(std::uint64_t move, std::uint64_t rev) {
  // flip
  m_black ^= move | rev;
  m_white ^= rev;
}

//-----------------------------------------------------------------------------

} // reversi6

//-----------------------------------------------------------------------------
