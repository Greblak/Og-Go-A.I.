#include "GoBlock.h"

GoBlock::GoBlock(GoBoard& b, int color, GoPoint anchor):color(color),anchor(anchor),board(b)
{

}

GoBlock::~GoBlock()
{

}
int GoBlock::Liberties() const
{
	return board.Liberties(anchor);
}