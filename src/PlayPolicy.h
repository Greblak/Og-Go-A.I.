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

  const std::vector<int> FindPossibleLocalMoves(GoBoard* board);
  const bool MatchAny(GoBoard* board, const int pos, const int color);
  const bool TestAllHane(GoBoard* board, const int pos, const int color, const int dirUp);
  const bool TestAllCut(GoBoard* board, const int pos, const int color, const int dirUp);

  //Patterns
  /**
   * All patterns described below are noted as follows and are for demonstration purposes seen from blacks point of view.
   *
   * T - Any to play at(empty)
   * B - Black to play at(empty)
   * W - White to play at(empty)
   * O - black
   * X - white
   * . - empty
   * ? - irrelevant
   * o - black or empty
   * x - white or empty
   *
   * Hane 1
   * O X O
   * . T .
   * ? ? ?
   *
   * Hane 2
   * O X .
   * . T .
   * ? . ?
   *
   * Hane 3
   * O X ?
   * B T .
   * ? . ?
   *
   * Hane 4 - Note that black to play. White should not play this
   * O X X
   * . B .
   * ? . ?
   *
   * Cut 1 matches if the first but NOT the 2nd and 3rd are matched
   *
   * 0 X ?		O X ?		O X ?
   * X T ?		X T X		X T .
   * ? ? ? 		? . ?		? X ?
   *
   * Cut 2
   * ? O ?
   * X T X
   * o o o
   *
   * Edge patterns. The bottom line is the edge. Returns true on any match
   *
   * O . ?		? O ?		? O X		? O X		? O X
   * X T ?		O T X		? B ?		? W O		X W O
   **/
  const bool TestHane1(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);
  const bool TestHane2(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);
  const bool TestHane3(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);
  const bool TestHane4(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);

  const bool TestCut1(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);
  const bool TestCut2(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);

  const bool TestEdge(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight);

private:
  GoBoard* board;
  const int getRightDirection(const int dirNorth) const; ///< Get the correct right direction based on the up direction parameter. Given north, this will output east. Given east, this will output south etc.
  const int getLeftDirection(const int dirNorth) const; ///< Same as getRightDirection, but returns left direction
  const int getOpponentColor(int color) const; ///< Converts black to white and white to black.
};

#endif /* PLAYPOLICY_H_ */
