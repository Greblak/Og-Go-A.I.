/*
 * SimpleRandomAI.h
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */

#ifndef SIMPLERANDOMAI_H_
#define SIMPLERANDOMAI_H_

#include "GoPoint.h"
#include "GoGame.h"

enum GameWinner
{
  BLACK_WIN = S_BLACK,
  WHITE_WIN = S_WHITE
};

class SimpleRandomAI
{
public:
  SimpleRandomAI();
  virtual
  ~SimpleRandomAI();

  GoPoint generateMove(const int color, GoGame* game); ///< Generate single move based on game
  GoPoint generateMove(const int color, GoBoard* board); ///< Generate single move based on board
  /**
   * Plays random moves until game is finished. Returns GameWinner
   **/
  int simulateGame(GoBoard* board);
};

#endif /* SIMPLERANDOMAI_H_ */
