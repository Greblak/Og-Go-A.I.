#include "Log.h"
#include "GoBoard.h"
#include "GoPoint.h"

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
	GoMove* m = new GoMove(color, &p ,0);
	return m;
}

const GoMove* GoBoard::Play(GoPoint p)
{
	State.stones[Pos(p)] = p.color;
	return Play(p, CurrentPlayer());
}

const int GoBoard::CurrentPlayer()
{
	return 0;
}

const int GoBoard::Pos( GoPoint p) const
{
	int x = p.x;
	int y = p.y;

	return y*BoardSize+x;
}

bool GoBoard::IsLegal(const GoPoint& p, int color)
{
	if(State.stones[Pos(p)]==NONE)
		return true;
	else
		return false;
}