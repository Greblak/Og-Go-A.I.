#include <sstream>
#include "Log.h"
#include "GoMove.h"
#include "GoGame.h"
#include "SGFEngine.h"
#include <cstdlib>

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
	SGFEngine::generateFile(this);
}

GoPoint GoGame::GenerateMove(int color)
{
	GoPoint point = GoPoint();
	point.color = color;
	point.x = rand()%19;
	point.y = rand()%19;

	while(!Board->IsLegal(point, point.color))
	{
		point.x = rand()%19;
		point.y = rand()%19;
	}

	Board->Play(point);

	return point;
}
