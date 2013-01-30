#include "GoBlock.h"

GoBlock::GoBlock()
{

}

GoBlock::~GoBlock()
{

}
const int GoBlock::Liberties() const
{
	return board->Liberties(anchor);
}