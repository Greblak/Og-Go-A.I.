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

int GoBoard::Pos( GoPoint p) const
{
	int x = p.x;
	int y = p.y;

	return y*BoardSize+x;
}

bool GoBoard::IsLegal(const GoPoint& p, int color)
{
	if(IsSuicide(p))
		return false;
	if(State.stones[Pos(p)]==NONE)
		return true;
	else
		return false;
}

bool GoBoard::IsSuicide(const GoPoint p) const
{
	int Nc = State.stones[North(p)];
	int Sc = State.stones[North(p)];
	int Wc = State.stones[North(p)];
	int Ec = State.stones[North(p)];
	if(Nc == NONE || Nc == p.color ||
		Sc == NONE || Sc == p.color ||
		Wc == NONE || Wc == p.color ||
		Ec == NONE || Ec == p.color ) //Does not account for setting in your own eye.
		return false;
	return true;
}

const int GoBoard::Liberties(const GoPoint p) const
{
	return 4;
}

int GoBoard::North(const GoPoint p) const
{
	return Pos(p)+POS_NS;
}

int GoBoard::South(const GoPoint p) const
{
	return Pos(p)-POS_NS;
}

int GoBoard::West(const GoPoint p) const
{
	return Pos(p)-POS_WE;
}

int GoBoard::East(const GoPoint p) const
{
	return Pos(p)+POS_WE;
}