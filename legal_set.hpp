/**
 * @file legal_set.hpp
 */
#ifndef REVERSI6_LEGAL_SET_HPP_
#define REVERSI6_LEGAL_SET_HPP_

#include "common.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

/**
 * legal set
 */
class LegalSet {
public:
  // constructor/destructor
  LegalSet() = delete;
  LegalSet(const LegalSet & rhs) = delete;
  LegalSet(LegalSet && rhs) = delete;
  LegalSet(info_t legals);
  virtual ~LegalSet() = default;

  // copy/move
  LegalSet & operator=(const LegalSet & rhs) = delete;
  LegalSet & operator=(LegalSet && rhs) = default;

//-----------------------------------------------------------------------------

  // empty
  bool is_empty() const;

  // pop
  info_t pop();

//-----------------------------------------------------------------------------

private:
  info_t m_legals;
};

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_LEGAL_SET_HPP_

//-----------------------------------------------------------------------------
