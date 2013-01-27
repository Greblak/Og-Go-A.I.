#include "Log.h"
#include "GoBoard.h"
#include "GoPoint.h"
#include <sstream>
#include "GTPEngine.h"

GoBoard::GoBoard(int size)
{
	if(size > BOARD_MAX_SIZE || size < BOARD_MINIMUM_SIZE)
		throw "Invalid boardsize.";
	BoardSize = size;
	for(int i = 0; i<(sizeof(State.stones)/sizeof(State.stones[0])); i++)
		State.stones[i] = NONE;
}

GoBoard::~GoBoard(void)
{

}

const int GoBoard::Size() const
{
	return BoardSize;
}

const GoMove* GoBoard::Play(GoPoint p, int color)
{
	State.stones[Pos(p)] = color;
	GoMove* m = new GoMove(color, &p ,0);
	return m;
}

const GoMove* GoBoard::Play(GoPoint p)
{
	return Play(p,p.color);
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
	return 4;
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