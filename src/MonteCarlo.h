/*
 * MonteCarlo.h
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */

#ifndef MONTECARLO_H_
#define MONTECARLO_H_

#include "GoPoint.h"
#include "GoGame.h"
#include "GoBoard.h"
#include "SimpleRandomAI.h"

class MonteCarlo : public SimpleRandomAI
{
public:
  MonteCarlo(int searchWidth, int searchDepth);
  virtual
  ~MonteCarlo();

  GoPoint generateMove(int color, GoGame* game);

private:
  const int searchDepth;
  const int searchWidth;
};

#endif /* MONTECARLO_H_ */
