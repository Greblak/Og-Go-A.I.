#include "Log.h"
#include "GoBoard.h"
#include "GoPoint.h"
#include "GoBlock.h"
#include "GoState.h"
#include <sstream>
#include "GTPEngine.h"

GoBoard::GoBoard(int size)
{
	if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
		throw "Invalid boardsize.";
	BoardSize = size;
	for(int i = 0; i<(sizeof(State.stones)/sizeof(State.stones[0])); i++)
	{
		State.stones[i] = NONE;
		std::fill(State.numNeighbours[S_BLACK], State.numNeighbours[S_BLACK]+BOARD_MAX_SIZE*BOARD_MAX_SIZE, 0);
		std::fill(State.numNeighbours[S_WHITE], State.numNeighbours[S_WHITE]+BOARD_MAX_SIZE*BOARD_MAX_SIZE, 0);
		if(IsBorder(i))
		{
			State.numNeighboursEmpty[i] = 3;
		}
		else if(IsCorner(i))
		{
			State.numNeighboursEmpty[i]= 2;
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

const bool GoBoard::Occupied(GoPoint p) const
{
	if(State.stones[Pos(p)] != NONE)
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
	
}

void GoBoard::KillDeadBlocks()
{

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
	State.stones[point] = color;
	--State.numNeighboursEmpty[point-POS_WE];
	--State.numNeighboursEmpty[point+POS_WE];
	--State.numNeighboursEmpty[point-POS_NS];
	--State.numNeighboursEmpty[point+POS_NS];
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
			return State.blockPointers[Pos(p)]->Liberties();
	return -1;
}

int GoBoard::North(const GoPoint p) const
{
	return Pos(p)+POS_NS;
}

int GoBoard::South(const GoPoint p) const
{
	return Pos(p)-POS_NS;
}

int GoBoard::West(const GoPoint p) const
{
	return Pos(p)-POS_WE;
}

int GoBoard::East(const GoPoint p) const
{
	return Pos(p)+POS_WE;
}

void GoBoard::DisplayCurrentState() const
{
	std::cout<<"=1 \n Showing current gamestate\n";
	for(int i = 0; i<BOARD_MAX_SIZE;i++)
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