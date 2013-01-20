#include <sstream>
#include "Log.h"
#include "GoGame.h"
#include <cstdlib>

GoGame::GoGame(int boardSize)
{
	Board = new GoBoard(boardSize);
}

GoGame::~GoGame()
{

}

void GoGame::Play(int color, int x, int y)
{
	
}

GoPoint GoGame::GenerateMove(int color)
{
	GoPoint point = GoPoint();
	point.color = color;
	point.x = rand()%19;
	point.y = rand()%19;
	bool t = Board->IsLegal();

	while(!Board->IsLegal(point, point.color))
	{
		point.x = rand()%19;
		point.y = rand()%19;
	}

	Board->Play(point);

	return point;
}