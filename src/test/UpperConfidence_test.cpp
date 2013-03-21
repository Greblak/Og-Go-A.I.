/*
 * UpperConfidence_test.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#include "UpperConfidence.h"
#include "GoGame.h"
#include "GoBoard.h"

void TEST_UpperConfidence()
{
  GoGame g(9);
  g.Play(S_BLACK,1,1);
  g.Play(S_BLACK,3,1);
  g.Play(S_WHITE,2,1);

  GoGame gg(9);
  gg.Board->resetAndReplayMoves(g.Board);

}
