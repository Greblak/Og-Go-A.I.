/*
 * PlayPolicy.h
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#ifndef PLAYPOLICY_H_
#define PLAYPOLICY_H_

#include "GoBoard.h"

class PlayPolicy
{
public:
  PlayPolicy();
  virtual
  ~PlayPolicy();

  const bool MatchAny(GoBoard* board, const int pos, const int color);
  const bool TestAllHane(GoBoard* board, const int pos, const int color, const int dirUp);

  //Patterns
  const bool TestHane1(GoBoard* board, const int pos, const int color, const int dirUp);
  const bool TestHane2(GoBoard* board, const int pos, const int color, const int dirUp);
  const bool TestHane3(GoBoard* board, const int pos, const int color, const int dirUp);
  const bool TestHane4(GoBoard* board, const int pos, const int color, const int dirUp);

private:
  GoBoard* board;
  const int getRightDirection(const int dirNorth) const;
  const int getOpponentColor(int color) const;
};

#endif /* PLAYPOLICY_H_ */
