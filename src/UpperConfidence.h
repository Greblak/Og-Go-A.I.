/*
 * UpperConfidence.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef UPPERCONFIDENCE_H_
#define UPPERCONFIDENCE_H_
#include "MonteCarlo.h"

class UpperConfidence
{
public:
  UpperConfidence(int searchWidth, int searchDepth);
  GoPoint generateMove(int color, GoGame* game);
  const float simulateMove(const GoPoint& move);
  virtual
  ~UpperConfidence();

private:
  const int searchDepth;
  const int searchWidth;
  const float expRatio;
  GoGame* game;
  int color;

};

#endif /* UPPERCONFIDENCE_H_ */
