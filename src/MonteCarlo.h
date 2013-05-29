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
#include "UpperConfidence.h"
#include "GoGame.h"

class MonteCarlo : public SimpleRandomAI
{
public:
  MonteCarlo(int moves, int simulations);
  virtual
  ~MonteCarlo();

  GoPoint generateMove(int color, GoGame* game); ///< Generate single move
  std::vector<UCBrow> generateMCTable(std::vector<int> moves, int color, GoGame* game); ///< Generate UCB table 
private:
  const int numSimulations; ///< Number of simulations to run
  const int numMoves; ///< Number of added moves.
};

#endif /* MONTECARLO_H_ */
