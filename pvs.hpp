/**
 * @file pvs.hpp
 */
#ifndef REVERSI6_PVS_HPP_
#define REVERSI6_PVS_HPP_

//-----------------------------------------------------------------------------

namespace reversi6 {

class Board;

//-----------------------------------------------------------------------------

/**
 * Principal Variation Search
 */
class Pvs {
public:
  // constructor
  Pvs() = default;

  // destructor
  virtual ~Pvs() = default;

  // copy
  Pvs(const Pvs & rhs) = delete;
  Pvs & operator=(const Pvs & rhs) = delete;

  // move
  Pvs(Pvs && rhs) = delete;
  Pvs & operator=(Pvs && rhs) = delete;

//-----------------------------------------------------------------------------

  // root search
  int search(Board board);

  // alpha-beta search
  int search(Board board, int alpha, int beta);
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_PVS_HPP_

//-----------------------------------------------------------------------------
