/**
 * @file pvs.cpp
 */
#include "pvs.hpp"
#include "board.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

constexpr int MIN_SCORE = -32;
constexpr int MAX_SCORE = +32;
constexpr int TH_NEXT_SORT = 4;

//-----------------------------------------------------------------------------

// root search
int Pvs::search(Board board) {
  return search(board, MIN_SCORE, MAX_SCORE);
}

// alpha-beta search
int Pvs::search(Board board, int alpha, int beta) {
  // one empty
  if (board.play_to_end_game_if_one_empty()) {
    return board.evaluate();
  }

  // generate next
  std::vector<Board> next_vec;
  if (TH_NEXT_SORT >= board.count_empties()) {
    next_vec = board.generate_next_board_vector();
  } else {
    next_vec = board.generate_next_board_sorted_vector();
  }

  // no move
  if (next_vec.empty()) {
    // pass
    board.pass();

    // opponent
    if (board.has_legals()) {
      return - search(board, -beta, -alpha);
    } else {
      // end game
      return - board.evaluate();
    }
  }

  // pv node
  auto it(next_vec.cbegin());
  alpha = std::max(alpha, - search(*it, -beta, -alpha));

  // beta cut
  if (alpha >= beta) {
    return alpha;
  }

  for (++it; it != next_vec.cend(); ++it) {
    // null window search
    const int score = - search(*it, -alpha - 1, -alpha);

    // beta cut
    if (score >= beta) {
      return score;
    }

    // full window search
    if (score > alpha) {
      alpha = - search(*it, -beta, -score);

      // beta cut
      if (alpha >= beta) {
        break;
      }
    }
  }

  return alpha;
}

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------
