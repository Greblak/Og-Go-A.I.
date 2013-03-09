#include "GoBlock.h"
#include <iostream>
#include "Log.h"
#include <sstream>
GoBlock::GoBlock()
{
  LOG_DEBUG << "Created block at "<<this;
}

GoBlock::~GoBlock()
{	
  //Remove itself from Board:
  LOG_DEBUG << "Block deconstructor intialized";
  for(BlockListIterator it = board->blocks.begin(); it != board->blocks.end(); it++)
    {
      if((*it) == (this))
        {
          it = board->blocks.erase(it);
          it = board->blocks.end();
        }
    }
  board = 0;
  LOG_DEBUG << "Block deconstructor complete. Destroyed:"<<this;
}
const int GoBlock::Liberties() const
{
  LOG_DEBUG << "Libs for block at "<< this<< " : "<<board->Liberties(anchor);
  return board->Liberties(anchor);
}
void GoBlock::ImportBlock(GoBlock* block)
{

  LOG_DEBUG << "Importing block "<<block->anchor<< " into " <<anchor;
  int newLiberties = 0;
  for(std::list<int>::iterator it = block->stones.begin(); it != block->stones.end(); it++)
    {
      LOG_DEBUG << "Checking uncommon liberties for stone at "<<(*it)<<std::endl<<"Numblibs for stone: "<<board->State.numNeighboursEmpty[(*it)];
      if(board->State.stones[board->North(*it)] == NONE && !board->IsLibertyOfBlock(board->North(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->South(*it)] == NONE && !board->IsLibertyOfBlock(board->South(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->East(*it)] == NONE && !board->IsLibertyOfBlock(board->East(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->West(*it)] == NONE && !board->IsLibertyOfBlock(board->West(*it),anchor))
         ++newLiberties;
      LOG_DEBUG << "Current unique liberties" <<newLiberties<<std::endl;
      //Import all stones
      stones.push_back((*it));
      board->State.blockPointers[(*it)] = this;
    }
  LOG_DEBUG << "Libs for this block: "<<liberties<< " + Unique libs for other block: "<<newLiberties;

  liberties += newLiberties;
  LOG_DEBUG << "Blocks joined. "<<" - New number of liberties: "<<liberties;
  delete block;
}
void GoBlock::RemoveStones()
{
  for(std::list<int>::iterator it = stones.begin(); it!=stones.end(); it++)
    {
      LOG_DEBUG << "Deleting stone at position: "<<(*it);
      if(board->State.blockPointers[(*it)] == this)
        board->State.blockPointers[(*it)] == 0;
      board->RemoveStone(*it);
    }
  LOG_DEBUG << "All stones removed";
}
