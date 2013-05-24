#include "GoBlock.h"
#include <iostream>
#include "Log.h"
#include <sstream>
#include <list>
GoBlock::GoBlock():liberties(0),anchor(-1),board(0),color(NONE),lastStone(0)
{
  for(int i = 0; i<BLOCK_MAX_STONES;++i)
  		stones[i] = -1;

  reset();
}

GoBlock::~GoBlock()
{	
  //Remove itself from Board:
//  for(BlockListIterator it = board->blocks.begin(); it != board->blocks.end(); it++)
//    {
//      if((*it) == (this))
//        {
//          it = board->blocks.erase(it);
//          it = board->blocks.end();
//        }
//    }
//  RemoveStones();
  board = 0;
}
const int GoBlock::Liberties() const
{
  return liberties;
//  return board->Liberties(anchor);
}
void GoBlock::ImportBlock(GoBlock* block)
{
  if(block == 0x0)
    return;
  int newLiberties = 0;
  int i = 0;
  while(block->stones[i] != -1)
    {
//      if(board->North(*it) != -1 && board->State.stones[board->North(*it)] == NONE && !board->IsLibertyOfBlock(board->North(*it),anchor))
//        ++newLiberties;
//      if(board->South(*it) != -1 && board->State.stones[board->South(*it)] == NONE && !board->IsLibertyOfBlock(board->South(*it),anchor))
//        ++newLiberties;
//      if(board->West(*it) != -1 && board->State.stones[board->East(*it)] == NONE && !board->IsLibertyOfBlock(board->East(*it),anchor))
//        ++newLiberties;
//      if(board->East(*it) != -1 && board->State.stones[board->West(*it)] == NONE && !board->IsLibertyOfBlock(board->West(*it),anchor))
//        ++newLiberties;
      newLiberties += board->FindUniqueLiberties(block->stones[i], board->State.blockPointers[anchor]);
      //Import all stones
      addStone(block->stones[i]);
      board->State.blockPointers[block->stones[i]] = this;
      ++i;
    }

  liberties += newLiberties;
  block->reset();
  ++i;
}
void GoBlock::RemoveStones()
{
	int i = 0;
	  while(i<lastStone)
	    {
      board->RemoveStone(stones[i]);
      ++i;
    }
}

const int GoBlock::LastLiberty() const
{
	int i = 0;
	while(i<lastStone)
		    {
      if(board->IsEmpty(board->North(stones[i])))
              return board->North(stones[i]);
      if(board->IsEmpty(board->South(stones[i])))
              return board->South(stones[i]);
      if(board->IsEmpty(board->West(stones[i])))
              return board->West(stones[i]);
      if(board->IsEmpty(board->East(stones[i])))
              return board->East(stones[i]);
      ++i;
    }
  return -1;
}

void GoBlock::addStone(int pos)
{
	stones[lastStone++] = pos;
}

void GoBlock::reset()
{
	liberties = 0;
	anchor = -1;
	board = 0;
	color = -1;
	lastStone = 0;
	for(int i = 0; i<BLOCK_MAX_STONES;++i)
		stones[i] = -1;
}
