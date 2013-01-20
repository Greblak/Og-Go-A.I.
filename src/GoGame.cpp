#include "GoGame.h"

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

const GoPoint GoGame::GenerateMove(int color)
{
	return GoPoint();
}