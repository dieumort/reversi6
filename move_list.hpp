/**
 * @file move_list.hpp
 */
#ifndef MOVE_LIST_HPP_
#define MOVE_LIST_HPP_

#include <cstdint>
#include <array>
#include "common.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

class Board;

//-----------------------------------------------------------------------------

/**
 * move list
 */
class MoveList {
public:
  // constructor/destructor
  MoveList() = delete;
  MoveList(const MoveList & rhs) = delete;
  MoveList(MoveList && rhs) = delete;
  MoveList(info_t moves, const Board & board);
  virtual ~MoveList() = default;

  // copy/move
  MoveList & operator=(const MoveList & rhs) = delete;
  MoveList & operator=(MoveList && rhs) = default;

//-----------------------------------------------------------------------------

  // size
  std::size_t size() const;

  // get move
  info_t operator[](std::size_t n) const;

//-----------------------------------------------------------------------------

private:
  std::array<info_t, MAX_LEGALS> m_array;
  std::size_t m_size;
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // MOVE_LIST_HPP_

//-----------------------------------------------------------------------------
