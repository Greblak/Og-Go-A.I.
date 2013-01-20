#include "GoBoard.h"

GoBoard::GoBoard(int size)
{
	if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
		throw "Invalid boardsize.";
	BoardSize = size;
}

GoBoard::~GoBoard(void)
{

}

const int GoBoard::Size() const
{
	return BoardSize;
}

const GoMove* GoBoard::Play(GoPoint p, int color)
{
	GoMove* m = new GoMove(color, p ,0);
	return m;
}

const GoMove* GoBoard::Play(GoPoint p)
{
	return Play(p, CurrentPlayer());
}

const int GoBoard::CurrentPlayer()
{
	return 0;
}