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
          break;
        }
    }
  while(counter < 361);
  if(counter == 361)
    {
      LOG_DEBUG << "No valid moves left";
      return GoPoint(-1,-1,NONE);
    }

  Play(point.color, point.x, point.y);

  return point;
}
