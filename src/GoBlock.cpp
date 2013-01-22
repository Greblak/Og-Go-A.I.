#include "GoBlock.h"

GoBlock::~GoBlock()
{

}
int GoBlock::Liberties() const
{
	return board.Liberties(anchor);
}