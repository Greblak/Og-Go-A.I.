#include "GoBlock.h"
#include <iostream>
#include "Log.h"
#include <sstream>
#include <list>
GoBlock::GoBlock():liberties(0),anchor(-1),board(0),color(NONE),lastStone(0)
{
//  LOG_DEBUG << "Created block at "<<this;
  for(int i = 0; i<BLOCK_MAX_STONES;++i)
  		stones[i] = -1;

  reset();
}

GoBlock::~GoBlock()
{	
  //Remove itself from Board:
//  LOG_DEBUG << "Block deconstructor intialized";
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
//  LOG_DEBUG << "Block deconstructor complete. Destroyed:"<<this;
}
const int GoBlock::Liberties() const
{
//  LOG_DEBUG << "Libs for block at "<< this<< " : "<<board->Liberties(anchor);
  return board->Liberties(anchor);
}
void GoBlock::ImportBlock(GoBlock* block)
{
//  LOG_DEBUG << "Importing block "<<block->anchor<< " into " <<anchor;
  int newLiberties = 0;
  int i = 0;
//  LOG_DEBUG << "LS: "<<lastStone;
  while(block->stones[i] != -1)
    {
//	  LOG_DEBUG << "i :"<<i<< " "<<block->stones[i];
//      LOG_DEBUG << "Checking uncommon liberties for stone at "<<block->stones[i]<<std::endl<<"Numblibs for stone: "<<board->State.numNeighboursEmpty[block->stones[i]];
//      if(board->North(*it) != -1 && board->State.stones[board->North(*it)] == NONE && !board->IsLibertyOfBlock(board->North(*it),anchor))
//        ++newLiberties;
//      if(board->South(*it) != -1 && board->State.stones[board->South(*it)] == NONE && !board->IsLibertyOfBlock(board->South(*it),anchor))
//        ++newLiberties;
//      if(board->West(*it) != -1 && board->State.stones[board->East(*it)] == NONE && !board->IsLibertyOfBlock(board->East(*it),anchor))
//        ++newLiberties;
//      if(board->East(*it) != -1 && board->State.stones[board->West(*it)] == NONE && !board->IsLibertyOfBlock(board->West(*it),anchor))
//        ++newLiberties;
      newLiberties += board->FindUniqueLiberties(block->stones[i], board->State.blockPointers[anchor]);
//      LOG_DEBUG << "Current unique liberties" <<newLiberties<<std::endl;
      //Import all stones
      addStone(block->stones[i]);
      board->State.blockPointers[block->stones[i]] = this;
      ++i;
    }
//  LOG_DEBUG << "Libs for this block: "<<liberties<< " + Unique libs for other block: "<<newLiberties;

  liberties += newLiberties;
  LOG_DEBUG << "Blocks joined. "<<" - New number of liberties: "<<liberties;
  block->reset();
  ++i;
}
void GoBlock::RemoveStones()
{
	int i = 0;
	  while(i<lastStone)
	    {
//      LOG_DEBUG << "Deleting stone at position: "<<stones[i];
      board->RemoveStone(stones[i]);
      ++i;
    }
//  LOG_DEBUG << "All stones removed";
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
