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
	UpperConfidence(void);
	UpperConfidence(int randomMoves, int numSimulations);
	UpperConfidence(std::vector<int> randomMoves, int numSimulations);
  GoPoint generateMove(int color, GoGame* game);
  const float simulateMove(int move);
  virtual
  ~UpperConfidence();

private:
  const int numSimulations;
  const int numRandMoves;
  const float expRatio;
  std::vector<int> preselRandMoves;
  GoBoard* workingBoard;
  GoGame* game;
  int color;

};

#endif /* UPPERCONFIDENCE_H_ */
