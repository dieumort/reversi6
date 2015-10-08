/**
 * @file pvs.hpp
 */
#ifndef REVERSI6_PVS_HPP_
#define REVERSI6_PVS_HPP_

//-----------------------------------------------------------------------------

namespace reversi6 {

class Board;

//-----------------------------------------------------------------------------

class Pvs {
public:
  // constructor/destructor
  Pvs() = default;
  Pvs(const Pvs & rhs) = delete;
  Pvs(Pvs && rhs) = delete;
  virtual ~Pvs() = default;

  // copy/move
  Pvs & operator=(const Pvs & rhs) = delete;
  Pvs & operator=(Pvs && rhs) = delete;

//-----------------------------------------------------------------------------

  // root search
  int search(Board b);

  // alpha-beta search
  int search(Board b, int alpha, int beta);

};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_PVS_HPP_

//-----------------------------------------------------------------------------
