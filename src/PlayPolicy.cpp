/*
 * PlayPolicy.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */

#include "PlayPolicy.h"
#include "GoBoard.h"

PlayPolicy::PlayPolicy()
{
  // TODO Auto-generated constructor stub

}

PlayPolicy::~PlayPolicy()
{
  // TODO Auto-generated destructor stub
}

const bool PlayPolicy::MatchAny(GoBoard* board, const int pos, const int color)
{
  int direction = board->POS_NS; //Init default value, to be safe :)
  for(int i = 0; i<4; ++i)
    {
      switch(i)
      {
      case(0):direction = board->POS_NS;
      break;
      case(1):direction = -board->POS_NS;
      break;
      case(2):direction = board->POS_WE;
      break;
      case(3):direction = -board->POS_WE;
      break;
      }

      if(TestAllHane(board,pos,color,direction)) // && TestAllCut
        return true;
    }
  if(TestEmpty(board,pos)) //Since
    return true;
  return false;
}
const bool PlayPolicy::TestAllHane(GoBoard* board, const int pos, const int color, const int dirUp)
{
  return TestHane1(board,pos,color,dirUp) || TestHane2(board,pos,color,dirUp) || TestHane3(board,pos,color,dirUp) || TestHane4(board,pos,color,dirUp);
}
const bool PlayPolicy::TestHane1(GoBoard* board, const int pos, const int color, const int dirUp)
{
  this->board = board;
  int dirRight = getRightDirection(dirUp);
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos+dirUp+dirRight, color)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight));
  return false;
}
const bool PlayPolicy::TestHane2(GoBoard* board, const int pos, const int color, const int dirUp)
{
  this->board = board;
  int dirRight = getRightDirection(dirUp);
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsEmpty(pos+dirUp+dirRight)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight));
  return false;
}

const bool PlayPolicy::TestHane3(GoBoard* board, const int pos, const int color, const int dirUp)
{
  this->board = board;
  int dirRight = getRightDirection(dirUp);
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos-dirRight, color)
      && board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirUp));
  return false;
}

const bool PlayPolicy::TestHane4(GoBoard* board, const int pos, const int color, const int dirUp)
{
  this->board = board;
  int dirRight = getRightDirection(dirUp);
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos+dirUp+dirRight, opp)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirUp));
  return false;
}

const bool PlayPolicy::TestEmpty(GoBoard* board, const int pos)
{
  this->board = board;
  int dirUp = board->POS_NS;
  int dirRight = getRightDirection(dirUp);
  return (
      board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirUp)
      && board->IsEmpty(pos+dirUp-dirRight)
      && board->IsEmpty(pos+dirUp+dirRight)
      && board->IsEmpty(pos-dirUp+dirRight)
      && board->IsEmpty(pos-dirUp-dirRight));
  return false;
}

const int PlayPolicy::getRightDirection(const int dirUp) const
{
  if(dirUp == board->POS_NS)
    return board->POS_WE;
  if(dirUp == -board->POS_NS)
    return -board->POS_WE;
  if(dirUp == board->POS_WE)
    return -board->POS_NS;
  if(dirUp == -board->POS_WE)
    return board->POS_NS;
  throw "Unknown direction";
}

const int PlayPolicy::getOpponentColor(int color) const
{
  return color == S_BLACK ? S_WHITE : S_BLACK;
}
