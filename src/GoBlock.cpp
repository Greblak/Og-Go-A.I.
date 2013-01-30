#include "GoBlock.h"
#include <iostream>
GoBlock::GoBlock()
{

}

GoBlock::~GoBlock()
{	
	//Remove itself from Board:
	for(BlockListIterator it = board->blocks.begin(); it != board->blocks.end(); it++)
	{
		if((*it) == (this))
		{
			it = board->blocks.erase(it);
			it = board->blocks.end();
		}
	}
	board = 0;
}
const int GoBlock::Liberties() const
{
	return board->Liberties(anchor);
}
void GoBlock::ImportBlock(GoBlock* block)
{
	int commonLiberties = 0;
	for(std::list<int>::iterator it = block->stones.begin(); it != block->stones.end(); it++)
	{
		commonLiberties += board->FindCommonLiberties(*it,anchor);
		//Import all stones
		stones.push_back((*it));
		std::cout<<"Copied stone at position: "<<(*it)<<std::endl;
	}
	liberties += block->Liberties() - commonLiberties;
	std::cout<<"Blocks joined. New number of liberties: "<<liberties<<std::endl;
	delete block;
}
void GoBlock::RemoveStones()
{
	for(std::list<int>::iterator it = stones.begin(); it!=stones.end(); it++)
	{
		std::cout<<"Deleting stone at position: "<<(*it)<<std::endl;
		board->State.stones[(*it)] = NONE;
	}
}
