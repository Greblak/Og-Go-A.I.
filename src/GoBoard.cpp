#include "Log.h"
#include "GoBoard.h"
#include "GoPoint.h"
#include "GoBlock.h"
#include "GoState.h"
#include <sstream>
#include "GTPEngine.h"

#include <iostream>

GoBoard::GoBoard(int size)
{
	if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
		throw "Invalid boardsize.";
	BoardSize = size;

	//Initializing board
	for(int i = 0; i<(BOARD_MAX_SIZE*BOARD_MAX_SIZE); i++)
	{
		State.stones[i] = NONE;
	
		State.numNeighbours[S_WHITE][i] = 0;
		State.numNeighbours[S_BLACK][i] = 0;
		
		if(IsCorner(i))
		{
			State.numNeighboursEmpty[i]= 2;
			std::cout<<"Corner: "<<i<<std::endl;
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
	if(State.stones[p] != NONE)
		return true;
	return false;
}

bool GoBoard::IsRealPoint(GoPoint p) const
{
	if(Pos(p) <= BOARD_MAX_SIZE*BOARD_MAX_SIZE && Pos(p) >= 0)
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

void GoBoard::UpdateBlocks(int pos, int color)
{
	int boardColor = color == S_BLACK? B_BLACK : B_WHITE;
	//Remove blockliberties of opposite color
	if(North(pos) != -1 && IsLibertyOfBlock(pos,North(pos)))
		State.blockPointers[North(pos)]->liberties--;
	if(South(pos) != -1 && IsLibertyOfBlock(pos,South(pos)))
		State.blockPointers[South(pos)]->liberties--;
	if(West(pos) != -1 && IsLibertyOfBlock(pos,West(pos)))
		State.blockPointers[West(pos)]->liberties--;
	if(East(pos) != -1 && IsLibertyOfBlock(pos,East(pos)))
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
		std::cout<<"Original libs: "<<b->liberties<<std::endl;
		
	}
	else //Not solo. Attach to block
	{
		int commonLiberties = 0;
		if (South(pos) != -1 && State.stones[South(pos)] == boardColor)
		{
			commonLiberties = FindCommonLiberties(pos,South(pos));
			State.blockPointers[pos] = State.blockPointers[South(pos)];
		}
		else if (North(pos) != -1 && State.stones[North(pos)] == boardColor)
		{
			commonLiberties = FindCommonLiberties(pos,North(pos));
			State.blockPointers[pos] = State.blockPointers[North(pos)];
		}
		else if (West(pos) != -1 && State.stones[West(pos)] == boardColor)
		{
			commonLiberties = FindCommonLiberties(pos,West(pos));
			State.blockPointers[pos] = State.blockPointers[West(pos)];
		}
		else if (East(pos) != -1 && State.stones[East(pos)] == boardColor)
		{
			commonLiberties = FindCommonLiberties(pos,East(pos));
			State.blockPointers[pos] = State.blockPointers[East(pos)];
		}
		
		std::cout<<"Old libcount: "<<State.blockPointers[pos]->liberties;
		State.blockPointers[pos]->liberties += State.numNeighboursEmpty[pos] - commonLiberties; //Subtract one since the new stone is always placed on a liberty for the block.
		std::cout<< " Stone liberties :"<<State.numNeighboursEmpty[pos] <<" Common: "<<commonLiberties<< " New liberty total: "<<State.blockPointers[pos]->liberties<<std::endl; 
		
		//TODO: Handle block joining to prevent counting common liberties twice. 
		
	}
	
	State.blockPointers[pos]->stones.push_back(pos);
	
	KillDeadBlocks();

}

void GoBoard::KillDeadBlocks()
{
	for(BlockListIterator it = blocks.begin(); it != blocks.end(); it++)
	{
		if((*it)->Liberties() == 0)
		{
			GoBlock* killblock = (*it);
			it = blocks.erase(it);
			delete killblock;
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
		std::cout<<point<<" is a liberty of the group at "<<North(point)<< "with anchor in "<<anchor<<std::endl;
		return true;
	}
	if(South(point) != -1 && State.blockPointers[South(point)] == block && State.blockPointers[South(point)] != State.blockPointers[point])
	{
		std::cout<<point<<" is a liberty of the group at "<<South(point)<< "with anchor in "<<anchor<<std::endl;
		return true;
	}
	if(West(point) != -1 && State.blockPointers[West(point)] == block && State.blockPointers[West(point)] != State.blockPointers[point])
	{
		std::cout<<point<<" is a liberty of the group at "<<West(point)<< "with anchor in "<<anchor<<std::endl;
		return true;
	}
	if(East(point) != -1 && State.blockPointers[East(point)] == block && State.blockPointers[East(point)] != State.blockPointers[point])
	{
		std::cout<<point<<" is a liberty of the group at "<<East(point)<< "with anchor in "<<anchor<<std::endl;
		return true;
	}
	return false;
}

const int GoBoard::FindCommonLiberties(const int point, const int anchor) const
{
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
		int w = West(point);
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

int GoBoard::Pos( GoPoint p) const
{
	int x = p.x;
	int y = p.y;
	return y*BoardSize+x;
}

bool GoBoard::IsLegal(const GoPoint& p, int color)
{
	if(IsSuicide(p))
		return false;
	if(State.stones[Pos(p)]==NONE)
		return true;
	else
		return false;
}

bool GoBoard::IsSuicide(const GoPoint p) const
{
	int Nc = State.stones[North(p)];
	int Sc = State.stones[North(p)];
	int Wc = State.stones[North(p)];
	int Ec = State.stones[North(p)];
	if(Nc == NONE || Nc == p.color ||
		Sc == NONE || Sc == p.color ||
		Wc == NONE || Wc == p.color ||
		Ec == NONE || Ec == p.color ) //Does not account for setting in your own eye.
		return false;
	return true;
}

const int GoBoard::Liberties(const GoPoint p) const
{
	if(IsRealPoint(p))
		if(Occupied(p))
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
	std::cout<<"=1 \n Showing current gamestate\n";
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
				std::stringstream ss;
				ss << "Wrong domain in board representation "<< State.stones[BOARD_MAX_SIZE*i+j];
				throw ss.str().c_str();
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