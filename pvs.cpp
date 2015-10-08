/**
 * @file pvs.cpp
 */
#include "pvs.hpp"
#include "common.hpp"
#include "board.hpp"
#include "move_list.hpp"

//-----------------------------------------------------------------------------

namespace reversi6 {

//-----------------------------------------------------------------------------

// root search
int Pvs::search(Board b) {
  return search(b, MIN_SCORE, MAX_SCORE);
}

// alpha-beta search
int Pvs::search(Board b, int alpha, int beta) {
  // one empty
  const info_t one_empty = b.get_move_if_one_empty();
  if (one_empty) {
    // can move
    if (b.can_move()) {
      b.set_move(one_empty);
      return - b.evaluate();
    } else {
      b.pass();

      // opponent can move
      if (b.can_move()) {
        b.set_move(one_empty);
        return b.evaluate();
      } else {
        // end game
        return - b.evaluate();
      }
    }
  }

  // generate moves
  MoveList move_list(b.generate_moves(), b);

  // no move
  if (0 == move_list.size()) {
    // pass
    b.pass();

    // opponent can move
    if (b.can_move()) {
      return - search(b, -beta, -alpha);
    } else {
      // end game
      return - b.evaluate();
    }
  }

  // pv node
  Board first_next(b);
  first_next.set_move(move_list[0]);
  const int first_score = - search(first_next, -beta, -alpha);
  if (first_score > alpha) {
    // max
    alpha = first_score;

    // beta cut
    if (alpha >= beta) {
      return alpha;
    }
  }

  for (std::size_t i = 1; i < move_list.size(); ++i) {
    // set move
    Board next(b);
    next.set_move(move_list[i]);

    // null window search
    const int score = - search(next, -alpha - 1, -alpha);

    // beta cut
    if (score >= beta) {
      return score;
    }

    // full window search
    if (score > alpha) {
      alpha = - search(next, -beta, -alpha);
    }

    // alpha beta cut
    if (alpha >= beta) {
      break;
    }
  }

  return alpha;
}

//-----------------------------------------------------------------------------

} // namespace reversi6

//-----------------------------------------------------------------------------
