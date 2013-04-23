#ifndef __GOGAME_H__
#define __GOGAME_H__

#include <vector>
#include "GoBoard.h"
#include "GoTimeHandler.h"

typedef std::vector<GoMove*>::const_iterator GoMoveIterator;

class GoGame
{
 private:
  

public:
	GoGame(int boardSize);
	~GoGame();

	GoBoard* Board; ///< The active board used in this game.

	GoTimeHandler TimeHandler;

	GoPoint GenerateMove(int color); ///< Used to trigger the AI to return a single move to be played
	void Play(int color, int x, int y); ///< Playes a move with the given parameters
	
//	std::vector<GoMove*> moves; ///< A list of moves made in this game. Used to reproduce in SGF files and copy game instances
};

#endif //#ifndef __GOGAME_H__
