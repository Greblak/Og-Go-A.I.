#include "GoBlock.h"
#include <iostream>
#include "Log.h"
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
  Log::Deb("Importing block",__FILE__,__LINE__);
  int newLiberties = 0;
  for(std::list<int>::iterator it = block->stones.begin(); it != block->stones.end(); it++)
    {

      std::cout<<"Checking uncommon liberties for stone at "<<(*it)<<std::endl<<"Numblibs for stone: "<<board->State.numNeighboursEmpty[(*it)]<<std::endl;
      if(board->State.stones[board->North(*it)] == NONE && !board->IsLibertyOfBlock(board->North(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->South(*it)] == NONE && !board->IsLibertyOfBlock(board->South(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->East(*it)] == NONE && !board->IsLibertyOfBlock(board->East(*it),anchor))
         ++newLiberties;
      if(board->State.stones[board->West(*it)] == NONE && !board->IsLibertyOfBlock(board->West(*it),anchor))
         ++newLiberties;
      std::cout<<"Current unique liberties" <<newLiberties<<std::endl;
      //Import all stones
      stones.push_back((*it));
    }
  std::cout<<"Libs for this block: "<<liberties<< " + Unique libs for other block: "<<newLiberties<<std::endl;
  liberties += newLiberties;
  std::cout<<"Blocks joined. "<<" - New number of liberties: "<<liberties<<std::endl;
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
