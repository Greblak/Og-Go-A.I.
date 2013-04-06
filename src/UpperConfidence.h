/*
 * UpperConfidence.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef UPPERCONFIDENCE_H_
#define UPPERCONFIDENCE_H_
#include "MonteCarlo.h"

struct UCBrow
{
	int pos;
	float expected;
	int timesPlayed;
};
class UpperConfidence
{
public:
<<<<<<< HEAD
  UpperConfidence(int searchWidth, int searchDepth);
  GoPoint generateMove(int color, GoGame* game);
=======
	UpperConfidence(void);
	UpperConfidence(int randomMoves, int numSimulations);
	UpperConfidence(std::vector<int> randomMoves, int numSimulations);
	GoPoint generateMove(int color, GoGame* game);
	GoPoint generateMove(std::vector<int> preselMoves);
	std::vector<UCBrow> generateUCBTable(int color, GoGame* game);
  std::vector<int> getPossibleMoves(int color, GoGame* game);
>>>>>>> d63b420... Parallelisation now works but is surprisingly slow. refs #27 @3h0
  const float simulateMove(int move);
  virtual
  ~UpperConfidence();

private:
  const int searchDepth;
  const int searchWidth;
  const float expRatio;
  GoBoard* workingBoard;
  GoGame* game;
  int color;

};

#endif /* UPPERCONFIDENCE_H_ */
