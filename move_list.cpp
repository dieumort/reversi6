/**
 * @file move_list.cpp
 */
#include <algorithm>
#include "move_list.hpp"
#include "board.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

constexpr info_t STATIC_ORDERING[] = {0x0000000000000200ULL, 0x0002000000000000ULL, 0x0000000000004000ULL, 0x0040000000000000ULL, 0x0000000002000000ULL, 0x0000000200000000ULL, 0x0000000000000800ULL, 0x0008000000000000ULL, 0x0000000000001000ULL, 0x0010000000000000ULL, 0x0000000040000000ULL, 0x0000004000000000ULL, 0x0000000004000000ULL, 0x0000000400000000ULL, 0x0000000000080000ULL, 0x0000080000000000ULL, 0x0000000000100000ULL, 0x0000100000000000ULL, 0x0000000020000000ULL, 0x0000002000000000ULL, 0x0000000000020000ULL, 0x0000020000000000ULL, 0x0000000000000400ULL, 0x0004000000000000ULL, 0x0000000000002000ULL, 0x0020000000000000ULL, 0x0000000000400000ULL, 0x0000400000000000ULL, 0x0000000000040000ULL, 0x0000040000000000ULL, 0x0000000000200000ULL, 0x0000200000000000ULL};

//-----------------------------------------------------------------------------

// constructor
MoveList::MoveList(info_t moves, const Board & board)
: m_array(), m_size(0) {
  // // no ordering
  // while (moves) {
  //   const info_t move = moves & (-moves);
  //   m_array[m_size++] = move;
  //   moves ^= move;
  // }

  // static ordering
  // for (std::size_t i = 0; i < 32; ++i) {
  //   if(STATIC_ORDERING[i] & moves) {
  //     m_array[m_size++] = STATIC_ORDERING[i];
  //   }
  // }

  // openess, next mobility ordering
  std::array<std::pair<int, info_t>, 32> move_array;
  while (moves) {
    const info_t m = moves & (-moves);
    //move_array[m_size].first = -board.get_openness(m);
    move_array[m_size].first = board.get_next_mobility(m);
    move_array[m_size++].second = m;
    moves ^= m;
  }

  // sort
  std::sort(move_array.begin(), move_array.begin() + m_size);

  // array
  for (std::size_t i = 0; i < m_size; ++i) {
    m_array[i] = move_array[i].second;
  }
}

//-----------------------------------------------------------------------------

// size
std::size_t MoveList::size() const {
  return m_size;
}

// get move
info_t MoveList::operator[](std::size_t n) const {
  return m_array[n];
}

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------
