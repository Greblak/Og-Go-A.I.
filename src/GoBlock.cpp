#include "GoBlock.h"

GoBlock::GoBlock(GoBoard& b):board(b)
{

}

GoBlock::~GoBlock()
{

}
int GoBlock::Liberties() const
{
	return board.Liberties(anchor);
}