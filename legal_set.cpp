/**
 * @file legal_set.cpp
 */
#include "legal_set.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

// constructor
LegalSet::LegalSet(info_t legals)
: m_legals(legals) {
}

//-----------------------------------------------------------------------------

// empty
bool LegalSet::is_empty() const {
  return !m_legals;
}

// pop
info_t LegalSet::pop() {
  const info_t legal = m_legals & (-m_legals);
  m_legals ^= legal;
  return legal;
}

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------
