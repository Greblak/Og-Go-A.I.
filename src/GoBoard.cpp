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
  if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
    throw "Invalid boardsize.";
  BoardSize = size;
 State.koPoint = NO_KO_POINT;
 LOG_DEBUG<< "KOPOINT SET TO "<<State.koPoint;
  //Initializing board
  for(int i = 0; i<(BOARD_MAX_SIZE*BOARD_MAX_SIZE); i++)
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
}

GoBoard::~GoBoard(void)
{

}

const bool GoBoard::IsBorder(int pos) const
{
  if(pos%BOARD_MAX_SIZE==0)
    return true;
  if(pos%BOARD_MAX_SIZE==18)
    return true;
  if(pos>= 0 && pos <=18)
    return true;
  if(pos<= BOARD_MAX_SIZE*BOARD_MAX_SIZE && pos >= BOARD_MAX_SIZE*BOARD_MAX_SIZE-19)
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
  if(p <= BOARD_MAX_SIZE*BOARD_MAX_SIZE && p >= 0)
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
  int uniqueLiberties;

  int lib;
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
    LOG_DEBUG << "Removed liberty for block at "<< North(pos);
    }
  if(South(pos) != -1 &&
      (State.blockPointers[South(pos)] != State.blockPointers[North(pos)]) //These are in place on west and east as well, to prevent removing too several liberties from the same group
      && IsLibertyOfBlock(pos,South(pos)))
    {
    State.blockPointers[South(pos)]->liberties--;
    LOG_DEBUG << "Removed liberty for block at "<< North(pos);
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
      LOG_DEBUG << "Old block("<<State.blockPointers[pos]->anchor<<") libcount: "<<State.blockPointers[pos]->liberties;
      //Subtract one since the new stone is always placed on a liberty for the block.
      State.blockPointers[pos]->liberties += uniqueLiberties;
      LOG_DEBUG <<  " Stone liberties :"<<State.numNeighboursEmpty[pos] <<" Unique: "<<uniqueLiberties<< " New block liberty total: "<<State.blockPointers[pos]->liberties;

      //TODO: Handle block joining
      //Detect neighboring blocks to perform join.
      //Has similar neighbor but not recently attached block
      GoBlock* copyBlock = NULL;
      if(State.stones[North(pos)] == boardColor && State.blockPointers[North(pos)] != State.blockPointers[pos])
        copyBlock = State.blockPointers[North(pos)];
      if(State.stones[South(pos)] == boardColor && State.blockPointers[South(pos)] != State.blockPointers[pos])
        copyBlock = State.blockPointers[South(pos)];
      if(State.stones[West(pos)] == boardColor && State.blockPointers[West(pos)] != State.blockPointers[pos])
        copyBlock = State.blockPointers[West(pos)];
      if(State.stones[East(pos)] == boardColor && State.blockPointers[East(pos)] != State.blockPointers[pos])
        copyBlock = State.blockPointers[East(pos)];
      if(copyBlock != 0)
        {
          std::vector<int> commonLiberties;
          GoBlock* curBlock = State.blockPointers[pos];
          curBlock->ImportBlock(copyBlock); //TODO: Not functioning properly.
          //TODO Find common liberties and prevent multiple counting
        }

    }
  LOG_DEBUG << "Stone added";
  State.blockPointers[pos]->stones.push_back(pos);
  KillSurroundingDeadBlocks(pos);

}

void GoBoard::KillSurroundingDeadBlocks(const int pos)
{
  //Reset kopoint
  State.koPoint = NO_KO_POINT;
  for(int i = 0; i<4; i++)
    {
      int blockPos = -1;
      if(i == 0 && IsRealPoint(North(pos)) && State.stones[North(pos)] != NONE)
	blockPos = North(pos);
      if(i == 1 && IsRealPoint(South(pos)) && State.stones[South(pos)] != NONE)
	blockPos = South(pos);
      if(i == 2 && IsRealPoint(West(pos)) && State.stones[West(pos)] != NONE)
	blockPos = West(pos);
      if(i == 3 && IsRealPoint(East(pos)) && State.stones[East(pos)] != NONE)
	blockPos = East(pos);
      if(blockPos == -1)
	continue;
      GoBlock* p_block = State.blockPointers[blockPos];
      if(p_block->Liberties() == 0)
	{
	  //Create ko-point if applicable(Single stone)
	  if(p_block->stones.size()==1)
	    {
	      State.koPoint = blockPos;
	      LOG_DEBUG << "Ko-point is now on "<<State.koPoint;
	    }

	  p_block->RemoveStones();
	  delete p_block;
	}
      i++;
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
  if(pos == 0 || pos == 18 || pos == BOARD_MAX_SIZE*BOARD_MAX_SIZE-1 || pos == BOARD_MAX_SIZE*BOARD_MAX_SIZE-19)
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

  LOG_DEBUG << "Playing color "<<p.color<<" at "<<p.x<<","<<p.y;
  AddStone(Pos(p),p.color);

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
  if(IsRealPoint(North(pos)))
    {
      ++State.numNeighboursEmpty[North(pos)];
      --State.numNeighbours[stoneColor][North(pos)];
      if(State.stones[North(pos)] != NONE)
	++State.blockPointers[North(pos)]->liberties;
    }
  if(IsRealPoint(South(pos)))
    {
      ++State.numNeighboursEmpty[South(pos)];
      --State.numNeighbours[stoneColor][South(pos)];
	if(State.stones[South(pos)] != NONE)
	  ++State.blockPointers[South(pos)]->liberties;
    }
  if(IsRealPoint(West(pos)))
    {
      ++State.numNeighboursEmpty[West(pos)];
      --State.numNeighbours[stoneColor][West(pos)];
      if(State.stones[West(pos)] != NONE)
	++State.blockPointers[West(pos)]->liberties;
    }
  if(IsRealPoint(East(pos)))
    {
      ++State.numNeighboursEmpty[East(pos)];
      --State.numNeighbours[stoneColor][East(pos)];
      if(State.stones[East(pos)] != NONE)
	++State.blockPointers[East(pos)]->liberties;
    }
  //Remove the stone from the board
  State.stones[pos] = NONE;
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
  if(State.numNeighboursEmpty[pos]>0)
    return false;

  //Allow "temporary suicide" if it leads to capture.
  if(Occupied(North(pos)) && State.stones[North(pos)] != boardColor && State.blockPointers[North(pos)]->liberties == 1)
    return false;
  if (Occupied(South(pos)) && State.stones[South(pos)] != boardColor && State.blockPointers[South(pos)]->liberties == 1)
    return false;
  if(Occupied(West(pos)) && State.stones[West(pos)] != boardColor && State.blockPointers[West(pos)]->liberties == 1) 
    return false;
  if(Occupied(East(pos)) && State.stones[East(pos)] != boardColor && State.blockPointers[East(pos)]->liberties == 1)
    return false;
  
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
  if(p%19==0)
    return -1;
  return p-POS_WE;
}

int GoBoard::East(const int p) const
{
  if(p%18==0 && p != 0)
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
  for(int i = BOARD_MAX_SIZE-1; i>=0;i--)
    {
      if(i+1 >= 10)
	std::cout<<"\n "<<(i+1);
      else
        std::cout<<"\n  "<<(i+1);
      for(int j = 0; j<BOARD_MAX_SIZE;j++)
        {
          if(State.stones[BOARD_MAX_SIZE*i+j] == NONE)
            std::cout<< " -";
          else if(State.stones[BOARD_MAX_SIZE*i+j] == B_BLACK)
            std::cout<< " O";
          else if(State.stones[BOARD_MAX_SIZE*i+j] == B_WHITE)
            std::cout<< " X";
          else
            {
              LOG_ERROR<< "Wrong domain in board representation "<< State.stones[BOARD_MAX_SIZE*i+j];
              throw "Domain error in board representation. See log file for details";
            }
        }
    }
  std::cout<<"\n    ";
  for(int k = 0; k<BOARD_MAX_SIZE; k++)
    {
      std::cout<<GTPEngine::ColumnIntToString(k)<<" ";
    }
  std::cout<<"\n\n\n\n\n";
}
