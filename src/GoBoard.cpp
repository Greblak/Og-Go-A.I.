#include "GoBoard.h"

GoBoard::GoBoard(int size):BoardSize(size)
{

}

GoBoard::~GoBoard(void)
{

}

const int GoBoard::Size() const
{
	return BoardSize;
}