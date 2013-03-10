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
  // TODO Auto-generated constructor stub

}

SimpleRandomAI::~SimpleRandomAI()
{
  // TODO Auto-generated destructor stub
}

GoPoint SimpleRandomAI::generateMove(const int color, GoGame* Game)
{
  GoPoint point = GoPoint();
  point.color = color;
  int counter = 0;
  do
    {
      point.x = rand()%Game->Board->Size();
      point.y = rand()%Game->Board->Size();
      ++counter;
      if(Game->Board->IsLegal(point, point.color) && !Game->Board->IsTrueEye(Game->Board->Pos(point), point.color+1)) //Legal move. Do more checks
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
