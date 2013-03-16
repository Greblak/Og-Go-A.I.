#include "Log.h"
#include "Config.h"
#include "GoBoard.h"
#include "GoPoint.h"
#include "GoBlock.h"
#include "GoState.h"
#include <sstream>
#include <vector>
#include "GTPEngine.h"

#include <iostream>


GoBoard::GoBoard(int size)
{
  LOG_VERBOSE <<"Initialising new board";
  if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
    throw "Invalid boardsize.";
  BoardSize = size;
  LOG_VERBOSE << "Board set to size "<<BoardSize;
  State.koPoint = NO_KO_POINT;

  BOARD_TOP_LEFT = size*size - size;
  BOARD_TOP_RIGHT = size*size - 1;
  BOARD_BOTTOM_LEFT = 0;
  BOARD_BOTTOM_RIGHT = size -1;

  POS_WE = 1;
  POS_NS = size;

  for(int i = 0; i<(Size()*Size()); i++)
    {
      State.stones[i] = NONE;


      State.numNeighbours[S_WHITE][i] = 0;
      State.numNeighbours[S_BLACK][i] = 0;

      if(IsCorner(i))
        {
          State.numNeighboursEmpty[i]= 2;
        }
      else if(IsBorder(i))
        {
          State.numNeighboursEmpty[i] = 3;
        }
      else //Mid board
        {
          State.numNeighboursEmpty[i] = 4;
        }
    }
  LOG_VERBOSE <<"Board initialised. Let's play!";
}

GoBoard::~GoBoard(void)
{

}

const bool GoBoard::IsBorder(int pos) const
{
  if(pos%Size()==0) // Left side
    return true;
  if(pos%Size()==Size()-1) //Right side
    return true;
  if(pos>= 0 && pos <=Size()-1) //Bottom side
    return true;
  if(pos<= BOARD_TOP_RIGHT && pos >= BOARD_TOP_LEFT)
    return true;
  return false;
}

const bool GoBoard::Occupied(const GoPoint p) const
{
  return Occupied(Pos(p));
}

const bool GoBoard::Occupied(const int p) const
{
  if(IsRealPoint(p) && State.stones[p] != NONE)
    return true;
  return false;
}

bool GoBoard::IsRealPoint(GoPoint p) const
{
  return IsRealPoint(Pos(p));
}

bool GoBoard::IsRealPoint(int p) const
{
  if(p <= Size()*Size() && p >= 0)
    return true;
  else
    return false;
}
const bool GoBoard::IsEmpty(GoPoint p) const
{
  return !Occupied(p);
}

void GoBoard::UpdateBlocks(const GoPoint p)
{
  UpdateBlocks(Pos(p), p.color);

}

const int GoBoard::FindUniqueLiberties(const int stone, const GoBlock* block) const
{
  int uniqueLiberties = 0;

  int lib = 0;
  for(int i = 0; i < 4; i++)
    {
      if(i == 0)
        lib = North(stone);
      else if(i == 1)
        lib = South(stone);
      else if(i == 2)
        lib = West(stone);
      else if(i == 3)
        lib = East(stone);
      if(State.stones[lib] == NONE)
        {
          if(State.blockPointers[North(lib)] != block &&
              State.blockPointers[South(lib)] != block &&
              State.blockPointers[West(lib)] != block &&
              State.blockPointers[East(lib)] != block)
            ++uniqueLiberties;
        }
    }
  return uniqueLiberties;
}

void GoBoard::UpdateBlocks(int pos, int color)
{
  int boardColor = color == S_BLACK? B_BLACK : B_WHITE;
  //Remove blockliberties of opposite color
  if(North(pos) != -1 && IsLibertyOfBlock(pos,North(pos)))
    {
      State.blockPointers[North(pos)]->liberties--;
      //    LOG_DEBUG << "Removed liberty for block at "<< North(pos);
    }
  if(South(pos) != -1 &&
      (State.blockPointers[South(pos)] != State.blockPointers[North(pos)]) //These are in place on west and east as well, to prevent removing too several liberties from the same group
      && IsLibertyOfBlock(pos,South(pos)))
    {
      State.blockPointers[South(pos)]->liberties--;
      //    LOG_DEBUG << "Removed liberty for block at "<< North(pos);
    }
  if(West(pos) != -1
      && (State.blockPointers[West(pos)] != State.blockPointers[North(pos)] && State.blockPointers[West(pos)] != State.blockPointers[South(pos)])
      && IsLibertyOfBlock(pos,West(pos)))
    State.blockPointers[West(pos)]->liberties--;
  if(East(pos) != -1
      && (State.blockPointers[East(pos)] != State.blockPointers[North(pos)] && State.blockPointers[East(pos)] != State.blockPointers[South(pos)] && State.blockPointers[East(pos)] != State.blockPointers[West(pos)])
      && IsLibertyOfBlock(pos,East(pos)))
    State.blockPointers[East(pos)]->liberties--;

  if(State.numNeighbours[color][pos] == 0) //Solo stone. Create new block
    {
      GoBlock* b = new GoBlock();
      blocks.push_back(b);
      b->anchor = pos;
      b->color = color;
      b->board = this;
      b->liberties= State.numNeighboursEmpty[pos];
      State.blockPointers[pos] = b;

      LOG_DEBUG << "Single stone places with liberties: "<<b->liberties;

    }
  else //Not solo. Attach to block
    {
      int uniqueLiberties = 0;
      if (South(pos) != -1 && State.stones[South(pos)] == boardColor)
        {
          //          commonLiberties = FindCommonLiberties(pos,South(pos));
          uniqueLiberties = FindUniqueLiberties(pos, State.blockPointers[South(pos)]);
          State.blockPointers[pos] = State.blockPointers[South(pos)];
        }
      else if (North(pos) != -1 && State.stones[North(pos)] == boardColor)
        {
          //          commonLiberties = FindCommonLiberties(pos,North(pos));
          uniqueLiberties = FindUniqueLiberties(pos, State.blockPointers[North(pos)]);
          State.blockPointers[pos] = State.blockPointers[North(pos)];
        }
      else if (West(pos) != -1 && State.stones[West(pos)] == boardColor)
        {
          //          commonLiberties = FindCommonLiberties(pos,West(pos));
          uniqueLiberties = FindUniqueLiberties(pos, State.blockPointers[West(pos)]);
          State.blockPointers[pos] = State.blockPointers[West(pos)];
        }
      else if (East(pos) != -1 && State.stones[East(pos)] == boardColor)
        {
          //          commonLiberties = FindCommonLiberties(pos,East(pos));
          uniqueLiberties = FindUniqueLiberties(pos, State.blockPointers[East(pos)]);
          State.blockPointers[pos] = State.blockPointers[East(pos)];
        }
      else
        LOG_ERROR <<"Something went terribly wrong";
      LOG_DEBUG << "Old block("<<State.blockPointers[pos]->anchor<<") libcount: "<<State.blockPointers[pos]->liberties;
      //Subtract one since the new stone is always placed on a liberty for the block.
      State.blockPointers[pos]->liberties += uniqueLiberties;
      LOG_DEBUG <<  " Stone liberties :"<<State.numNeighboursEmpty[pos] <<" Unique: "<<uniqueLiberties<< " New block liberty total: "<<State.blockPointers[pos]->liberties;


      //Detect neighboring blocks to perform join.
      //Has similar neighbor but not recently attached block
      if(State.stones[North(pos)] == boardColor && State.blockPointers[North(pos)] != State.blockPointers[pos])
        State.blockPointers[pos]->ImportBlock(State.blockPointers[North(pos)]);
      if(State.stones[South(pos)] == boardColor && State.blockPointers[South(pos)] != State.blockPointers[pos])
        State.blockPointers[pos]->ImportBlock(State.blockPointers[South(pos)]);
      if(State.stones[West(pos)] == boardColor && State.blockPointers[West(pos)] != State.blockPointers[pos])
        State.blockPointers[pos]->ImportBlock(State.blockPointers[West(pos)]);
      if(State.stones[East(pos)] == boardColor && State.blockPointers[East(pos)] != State.blockPointers[pos])
        State.blockPointers[pos]->ImportBlock(State.blockPointers[East(pos)]);
      //      if(copyBlock != 0)
      //        {
      //          std::vector<int> commonLiberties;
      //          GoBlock* curBlock = State.blockPointers[pos];
      //          curBlock->ImportBlock(copyBlock);
      //        }

    }
  LOG_DEBUG << "Stone added at: "<<pos;
  LOG_DEBUG << "Accessing "<<State.blockPointers[pos];
  State.blockPointers[pos]->stones.push_back(pos);
  KillSurroundingDeadBlocks(pos);

}

void GoBoard::KillSurroundingDeadBlocks(const int pos)
{
  //Reset kopoint
  State.koPoint = NO_KO_POINT;
  bool singleStoneKilledBefore = false;
  LOG_DEBUG << "Searching for dead blocks around "<<pos;
  for(int i = 0; i<4; i++)
    {
      int blockPos = -1;

      if(i == 0 && IsRealPoint(North(pos)) && State.stones[North(pos)] != NONE && State.stones[North(pos)] != State.stones[pos])
        blockPos = North(pos);
      else if(i == 1 && IsRealPoint(South(pos)) && State.stones[South(pos)] != NONE && State.stones[South(pos)] != State.stones[pos])
        blockPos = South(pos);
      else if(i == 2 && IsRealPoint(West(pos)) && State.stones[West(pos)] != NONE && State.stones[West(pos)] != State.stones[pos])
        blockPos = West(pos);
      else if(i == 3 && IsRealPoint(East(pos)) && State.stones[East(pos)] != NONE && State.stones[East(pos)] != State.stones[pos])
        blockPos = East(pos);
      //if(blockPos == -1)
      else
        continue;
      LOG_DEBUG << "Found potentially dead block at "<<blockPos;
      GoBlock* p_block = State.blockPointers[blockPos];
      LOG_DEBUG << "Block has " << p_block->Liberties()<<" liberties";
      if(p_block->Liberties() == 0)
        {
          //Create ko-point if applicable(Single stone)
          if(p_block->stones.size()==1)
            {
              if(!singleStoneKilledBefore)
                {
                  State.koPoint = blockPos;
                  LOG_DEBUG << "Ko-point is now on "<<State.koPoint;
                  singleStoneKilledBefore = true;
                }
              else
                {
                  State.koPoint = NO_KO_POINT;
                  LOG_DEBUG << "Multiple single stones killed in same move. No more ko for you!";
                }
            }

          p_block->RemoveStones(); //Remove stones from board and update liberties.
          delete p_block; //Free block from memory
        }
    }
}

const bool GoBoard::IsLibertyOfBlock(const int point, const int anchor) const
{
  if(!Occupied(anchor))
    return false;
  const GoBlock* block = State.blockPointers[anchor];
  if(North(point) != -1 && State.blockPointers[North(point)] == block && State.blockPointers[North(point)] != State.blockPointers[point])

    {
      //      std::cout<<point<<" is a liberty of the group at "<<North(point)<< "with anchor in "<<anchor<<std::endl;
      return true;
    }
  if(South(point) != -1 && State.blockPointers[South(point)] == block && State.blockPointers[South(point)] != State.blockPointers[point])
    {
      //      std::cout<<point<<" is a liberty of the group at "<<South(point)<< "with anchor in "<<anchor<<std::endl;
      return true;
    }
  if(West(point) != -1 && State.blockPointers[West(point)] == block && State.blockPointers[West(point)] != State.blockPointers[point])
    {
      //      std::cout<<point<<" is a liberty of the group at "<<West(point)<< "with anchor in "<<anchor<<std::endl;
      return true;
    }
  if(East(point) != -1 && State.blockPointers[East(point)] == block && State.blockPointers[East(point)] != State.blockPointers[point])
    {
      //      std::cout<<point<<" is a liberty of the group at "<<East(point)<< "with anchor in "<<anchor<<std::endl;
      return true;
    }
  return false;
}

const int GoBoard::FindCommonLiberties(const int point, const int anchor) const
{
  LOG_DEBUG << "Finding common liberties between point: "<<point<<" and block at "<<anchor;

  int commonLiberties = 0;
  if(North(point)!=-1 && point != anchor && IsLibertyOfBlock(North(point),anchor))
    {
      ++commonLiberties;
    }
  if(South(point)!=-1 && point != anchor && IsLibertyOfBlock(South(point),anchor))
    {
      ++commonLiberties;
    }
  if(West(point)!=-1 && point != anchor && IsLibertyOfBlock(West(point),anchor))
    {
      ++commonLiberties;
    }
  if(East(point)!=-1 && point != anchor && IsLibertyOfBlock(East(point),anchor))
    {
      ++commonLiberties;
    }
  return commonLiberties;
}

const bool GoBoard::IsCorner(int pos) const
{
  if(pos == 0 || pos == Size()-1|| pos == Size()*Size()-1 || pos == Size()*Size()-Size())
    return true;
  else
    return false;
}

const int GoBoard::Size() const
{
  return BoardSize;
}

const GoMove* GoBoard::Play(GoPoint p, int color)
{
  if(!IsLegal(p, p.color))
    throw "Illegal move";
  LOG_DEBUG << "\n\n\n";
  LOG_DEBUG << "Playing color "<<p.color<<" at "<<p.x<<","<<p.y;
  AddStone(Pos(p),p.color);
  LOG_DEBUG <<"Stone added";

  return 0;
}

const GoMove* GoBoard::Play(GoPoint p)
{
  return Play(p,p.color);
}

void GoBoard::AddStone(int point, int color)
{
  int boardColor = color == S_BLACK? B_BLACK : B_WHITE;
  //	int w = West(point);
  //	int e = East(point);
  //	int n = North(point);
  //	int s = South(point);
  State.stones[point] = boardColor;
  if(West(point) != -1)
    {

      --State.numNeighboursEmpty[West(point)];
      ++State.numNeighbours[color][West(point)];
    }
  if(East(point) != -1)
    {
      --State.numNeighboursEmpty[East(point)];
      ++State.numNeighbours[color][East(point)];
    }
  if(North(point) != -1)
    {
      --State.numNeighboursEmpty[North(point)];
      ++State.numNeighbours[color][North(point)];
    }
  if(South(point) != -1)
    {
      --State.numNeighboursEmpty[South(point)];
      ++State.numNeighbours[color][South(point)];
    }

  UpdateBlocks(point,color);
}

const int GoBoard::CurrentPlayer()
{
  return 0;
}

void GoBoard::RemoveStone(const int pos)
{
  int stoneColor = State.stones[pos] == B_BLACK? S_BLACK : S_WHITE;
  //Handling opened liberties
  LOG_DEBUG <<"Attempting to remove stone at "<<pos;
  if(IsRealPoint(North(pos)) && North(pos) != -1)
    {
      LOG_DEBUG <<"Checking north";
      //      LOG_DEBUG << "Handling renewed liberties for "<<North(pos)<< " Old libs: "<<State.blockPointers[North(pos)]->Liberties();
      ++State.numNeighboursEmpty[North(pos)];
      --State.numNeighbours[stoneColor][North(pos)];
      LOG_DEBUG <<"Checking north";
      if(State.stones[North(pos)] != NONE && !IsInSameBlock(pos,North(pos)))
        {
          LOG_DEBUG <<"Fixing north block";
          ++State.blockPointers[North(pos)]->liberties;
          LOG_DEBUG << "Block at : "<<East(pos)<<" has been updated with new libs: "<<State.blockPointers[North(pos)]->Liberties();
        }
      LOG_DEBUG <<"Done with north";
    }
  if(IsRealPoint(South(pos)) && South(pos) != -1)
    {
      LOG_DEBUG <<"Checking S";
      //      LOG_DEBUG << "Handling renewed liberties for "<<South(pos)<< " Old libs: "<<State.blockPointers[South(pos)]->Liberties();
      ++State.numNeighboursEmpty[South(pos)];
      --State.numNeighbours[stoneColor][South(pos)];
      if(State.stones[South(pos)] != NONE && !IsInSameBlock(pos,South(pos))
          && !IsInSameBlock(South(pos),North(pos)))
        {
          ++State.blockPointers[South(pos)]->liberties;
          LOG_DEBUG << "Block at : "<<East(pos)<<" has been updated with new libs: "<<State.blockPointers[South(pos)]->Liberties();
        }
    }
  if(IsRealPoint(West(pos)) && West(pos) != -1)
    {
      LOG_DEBUG <<"Checking W";
      //      LOG_DEBUG << "Handling renewed liberties for "<<West(pos)<< " Old libs: "<<State.blockPointers[West(pos)]->Liberties();
      ++State.numNeighboursEmpty[West(pos)];
      --State.numNeighbours[stoneColor][West(pos)];
      if(State.stones[West(pos)] != NONE && !IsInSameBlock(pos,West(pos))
          && !IsInSameBlock(West(pos),North(pos))
          && !IsInSameBlock(West(pos),South(pos)))
        {
          ++State.blockPointers[West(pos)]->liberties;
          LOG_DEBUG << "Block at : "<<East(pos)<<" has been updated with new libs: "<<State.blockPointers[West(pos)]->Liberties();
        }
    }
  if(IsRealPoint(East(pos)) && East(pos) != -1)
    {
      LOG_DEBUG <<"Checking E";
      //      LOG_DEBUG << "Handling renewed liberties for "<<East(pos)<< " Old libs: "<<State.blockPointers[East(pos)]->Liberties();
      ++State.numNeighboursEmpty[East(pos)];
      LOG_DEBUG <<"Checking E";
      --State.numNeighbours[stoneColor][East(pos)];
      LOG_DEBUG <<"Checking E";
      if(State.stones[East(pos)] != NONE && !IsInSameBlock(pos,East(pos))
          && !IsInSameBlock(East(pos),North(pos))
          && !IsInSameBlock(East(pos),South(pos))
          && !IsInSameBlock(East(pos),West(pos)))

        {
          ++State.blockPointers[East(pos)]->liberties;
          LOG_DEBUG << "Block at : "<<East(pos)<<" has been updated with new libs: "<<State.blockPointers[East(pos)]->Liberties();
        }
    }
  //Remove the stone from the board
  LOG_DEBUG << "Removing last references of stone at "<<pos;
  State.blockPointers[pos] = 0;
  State.stones[pos] = NONE;
}

const bool GoBoard::IsInSameBlock(const int pos1,const int pos2) const
{
  LOG_DEBUG <<"Comparing blocks at "<<pos1<<" and "<<pos2<<". In same block?";
  LOG_DEBUG <<"Comparing blocks at "<<State.blockPointers[pos1]<<" and "<<State.blockPointers[pos2]<<". In same block?";
  if(pos1 == -1 || pos2 == -1 || State.blockPointers[pos1] == 0 || State.blockPointers[pos2] == 0) //Nullpointer or invalid point
    return false;
  if(State.blockPointers[pos1] == State.blockPointers[pos2])
    {
      LOG_DEBUG <<"Pointers match!";
      return true;
    }
  else
    LOG_DEBUG <<"Pointers don't match!";
  return false;
}
int GoBoard::Pos( GoPoint p) const
{
  int x = p.x;
  int y = p.y;
  return y*BoardSize+x;
}

bool GoBoard::IsLegal(const GoPoint& p, int color)
{
  if(!IsRealPoint(Pos(p)))
    return false;
  if(Occupied(p))
    return false;
  LOG_DEBUG << "Testing to see if pos: "<<Pos(p)<<" matches ko point "<<State.koPoint;
  if(Pos(p) == State.koPoint)
    {
      LOG_DEBUG << "Illegal move due to Ko-rule";
      return false;
    }
  if(IsSuicide(p))
    {
      LOG_DEBUG << "Illegal move due to suicide";
      return false;
    }
  return true;
}

bool GoBoard::IsSuicide(const GoPoint p) const
{
  int boardColor = p.color == S_BLACK? B_BLACK : B_WHITE; //Converts GoPoint-Stonecolor to board color. Should be put in function...
  int pos = Pos(p);
  LOG_DEBUG << "Testing if "<<pos<< " is suicide";

  //Test for single stone
  if(State.numNeighboursEmpty[pos]>0)
    {

      LOG_DEBUG << "Legal. Single stone 1+ libs";
      return false;
    }

  //Test if joins block of same color
  if(Occupied(North(pos)) && State.stones[North(pos)] == boardColor && State.blockPointers[North(pos)]->Liberties()>1)
    {

      LOG_DEBUG << "Legal. Joined N block";
      return false;
    }
  if(Occupied(South(pos)) && State.stones[South(pos)] == boardColor && State.blockPointers[South(pos)]->Liberties()>1)
    {

      LOG_DEBUG << "Legal. Joined S block";
      return false;
    }
  if(Occupied(West(pos)) && State.stones[West(pos)] == boardColor && State.blockPointers[West(pos)]->Liberties()>1)
    {

      LOG_DEBUG << "Legal. Joined W block";
      return false;
    }
  if(Occupied(East(pos)) && State.stones[East(pos)] == boardColor && State.blockPointers[East(pos)]->Liberties()>1)
    {

      LOG_DEBUG << "Legal. Joined E block";
      return false;
    }

  //Allow "temporary suicide" if it leads to capture.
  if(Occupied(North(pos)) && State.stones[North(pos)] != boardColor && State.blockPointers[North(pos)]->liberties == 1)
    {

      LOG_DEBUG << "Legal. Led to N capture";
      return false;
    }
  if (Occupied(South(pos)) && State.stones[South(pos)] != boardColor && State.blockPointers[South(pos)]->liberties == 1)
    {

      LOG_DEBUG << "Legal. Led to S capture";
      return false;
    }
  if(Occupied(West(pos)) && State.stones[West(pos)] != boardColor && State.blockPointers[West(pos)]->liberties == 1) 
    {

      LOG_DEBUG << "Legal. Led to W capture";
      return false;
    }
  if(Occupied(East(pos)) && State.stones[East(pos)] != boardColor && State.blockPointers[East(pos)]->liberties == 1)
    {

      LOG_DEBUG << "Legal. Led to E capture";
      return false;
    }
  LOG_DEBUG << "This one is definetly suicidal!";
  return true;
}

const int GoBoard::Liberties(const GoPoint p) const
{
  if(IsRealPoint(p) && Occupied(p))
    return Liberties(Pos(p));
  return -1;
}

const int GoBoard::Liberties(int pos) const
{
  return State.blockPointers[pos]->liberties;
}

int GoBoard::North(const int p) const
{
  if(p>= BOARD_TOP_LEFT)
    return -1;
  return p+POS_NS;
}

int GoBoard::South(const int p) const
{
  if(p<=BOARD_BOTTOM_RIGHT)
    return -1;
  return p-POS_NS;
}

int GoBoard::West(const int p) const
{
  if(p%Size()==0)
    return -1;
  return p-POS_WE;
}

int GoBoard::East(const int p) const
{
  if((p+1)%Size()==0 && p != 0)
    return -1;
  return p+POS_WE;
}

int GoBoard::North(const GoPoint p) const
{
  return North(Pos(p));
}

int GoBoard::South(const GoPoint p) const
{
  return South(Pos(p));
}

int GoBoard::West(const GoPoint p) const
{
  return West(Pos(p));
}

int GoBoard::East(const GoPoint p) const
{
  return East(Pos(p));
}

void GoBoard::DisplayCurrentState() const
{
  LOG_OUT << "=1 \n Showing current gamestate\n";
  std::cerr<<"\n    ";
  for(int k = 0; k<Size(); k++)
    {
      std::cerr<<GTPEngine::ColumnIntToString(k)<<" ";
    }
  for(int i = Size()-1; i>=0;i--)
    {
      if(i+1 >= 10)
        std::cerr<<"\n "<<(i+1);
      else
        std::cerr<<"\n  "<<(i+1);
      for(int j = 0; j<Size();j++)
        {
          if(State.stones[Size()*i+j] == NONE)
            std::cerr<< " -";
          else if(State.stones[Size()*i+j] == B_BLACK)
            std::cerr<< " O";
          else if(State.stones[Size()*i+j] == B_WHITE)
            std::cerr<< " X";
          else
            {
              LOG_ERROR<< "Wrong domain in board representation "<< State.stones[Size()*i+j];
              throw "Domain error in board representation. See log file for details";
            }
        }
    }
  std::cerr<<"\n    ";
  for(int k = 0; k<Size(); k++)
    {
      std::cerr<<GTPEngine::ColumnIntToString(k)<<" ";
    }
  std::cerr<<"\n\n\n\n\n";
}

const bool GoBoard::IsTrueEye(const int point, const int boardColor)
{
  LOG_DEBUG << "Checking if "<<point <<" is a true eye";
  if(
      (North(point) == -1 || (State.stones[North(point)] == boardColor))
      && (South(point) == -1 || (State.stones[South(point)] == boardColor))
      && (West(point) == -1 || (State.stones[West(point)] == boardColor))
      && (East(point) == -1 || (State.stones[East(point)] == boardColor))
  )//Results in potential eye at point. May still be false
    {
      //Get diagonal colors
      int NW = West(point) != -1 && North(point) != -1 ? State.stones[North(West(point))] : -1;
      int NE = East(point) != -1 && North(point) != -1 ? State.stones[North(East(point))] : -1;
      int SW = West(point) != -1 && South(point) != -1 ? State.stones[South(West(point))] : -1;
      int SE = East(point) != -1 && South(point) != -1 ? State.stones[South(East(point))] : -1;

      LOG_DEBUG << NW << NE << SW <<SE<<State.stones[point];

      int numValid = 0;
      int numSameCol = 0;
      if(NW != -1)
        ++numValid;
      if(NE != -1)
        ++numValid;
      if(SW != -1)
        ++numValid;
      if(SE != -1)
        ++numValid;

      if(NW == boardColor)
        ++numSameCol;
      if(NE == boardColor)
        ++numSameCol;
      if(SW == boardColor)
        ++numSameCol;
      if(SE == boardColor)
        ++numSameCol;

      if((float)numSameCol / (float)numValid > 0.5)
        return true;
    }
  return false;
}
