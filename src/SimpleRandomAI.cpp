/*
 * SimpleRandomAI.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */

#include "SimpleRandomAI.h"
#include <cstdlib>
#include "Log.h"
#include "GoBoard.h"
#include "GoPoint.h"
SimpleRandomAI::SimpleRandomAI()
{

}

SimpleRandomAI::~SimpleRandomAI()
{

}

GoPoint SimpleRandomAI::generateMove(const int color, GoGame* Game)
{
  return generateMove(color, Game->Board);
}

GoPoint SimpleRandomAI::generateMove(const int color, GoBoard* Board)
{
  GoPoint point = GoPoint();
  point.color = color;
  int counter = 0;
  do
    {
      point.x = rand()%Board->Size();
      point.y = rand()%Board->Size();
      ++counter;
      if(Board->IsLegal(point, point.color) && !Board->IsTrueEye(Board->Pos(point), point.color+1)) //Legal move. Do more checks
        {
          break;
        }
    }
  while(counter < 1000);
  if(counter == 1000)
    {
      LOG_DEBUG << "No valid moves left";
      return POINT_PASS;
    }
  return point;
}

int SimpleRandomAI::simulateGame(GoBoard* board)
{
  int twoPasses = 0;
  while(twoPasses < 2)
    {
      GoPoint p =generateMove(board->CurrentPlayer(),board);
      if(p.IsPass())
        {
          ++twoPasses;
          continue;
        }
      twoPasses = 0;

      board->Play(p);
    }
  return 0; //TODO Deprecated. Should be void
}
