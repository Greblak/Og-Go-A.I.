#include "GoBlock.h"
#include <iostream>
GoBlock::GoBlock()
{

}

GoBlock::~GoBlock()
{
	for(std::list<int>::iterator it = stones.begin(); it!=stones.end(); it++)
	{
		std::cout<<"Deleting stone at position: "<<(*it)<<std::endl;
		board->State.stones[(*it)] = NONE;
	}
	
	board = 0;
}
const int GoBlock::Liberties() const
{
	return board->Liberties(anchor);
}