#ifndef __GOGAME_H__
#define __GOGAME_H__

#include <vector>
#include "GoBoard.h"

typedef std::vector<GoMove*>::const_iterator GoMoveIterator;

class GoGame
{
 private:

public:
	GoGame(int boardSize);
	~GoGame();

	GoBoard* Board;

	GoPoint GenerateMove(int color);
	void Play(int color, int x, int y);
	
	std::vector<GoMove*> moves;
};

#endif //#ifndef __GOGAME_H__
