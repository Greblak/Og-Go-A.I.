#include <sstream>
#include "Log.h"
#include "GoMove.h"
#include "GoGame.h"
#include "SGFEngine.h"
#include <cstdlib>
#include "Exception.h"

GoGame::GoGame(int boardSize)
{

  Board = new GoBoard(boardSize);
}

GoGame::~GoGame()
{
  delete Board;
}

void GoGame::Play(int color, int x, int y)
{
  GoPoint p = GoPoint(x,y,color);
  Board->Play(p);
  moves.push_back(new GoMove(p.color,p));
  //  SGFEngine::generateFile(this);
}

GoPoint GoGame::GenerateMove(int color)
{
  GoPoint point = GoPoint();
  point.color = color;
  int counter = 0;
  do
    {
      point.x = rand()%Board->Size();
      point.y = rand()%Board->Size();
      ++counter;
      if(Board->IsLegal(point, point.color)) //Legal move. Do more checks
        {
          if(
              (Board->North(point) == -1 || Board->State.stones[Board->North(point)] == point.color+1)
              && (Board->South(point) == -1 || Board->State.stones[Board->South(point)] == point.color+1)
              && (Board->West(point) == -1 || Board->State.stones[Board->West(point)] == point.color+1)
              && (Board->East(point) == -1 || Board->State.stones[Board->East(point)] == point.color+1)
          )//Placing in own eye. Dumb move.
            {
              LOG_DEBUG << "DONT WANNA PLAY EYE AT "<<point.x<<","<<point.y;
              continue;
            }
          break;
        }
    }
  while(counter < 1000);
  if(counter == 1000)
    {
      LOG_DEBUG << "No valid moves left";
      return GoPoint(-1,-1,NONE);
    }

  Play(point.color, point.x, point.y);

  return point;
}
