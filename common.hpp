/**
 * @file common.hpp
 */
#ifndef REVERSI6_COMMON_HPP_
#define REVERSI6_COMMON_HPP_

#include <cstdint>

//-----------------------------------------------------------------------------

namespace reversi6 {

using info_t = std::uint64_t;

constexpr int MIN_SCORE = -36;
constexpr int MAX_SCORE = +36;
constexpr int MAX_LEGALS = 18;

} // namespace reversi6

//-----------------------------------------------------------------------------

#endif // REVERSI6_COMMON_HPP_

//-----------------------------------------------------------------------------
