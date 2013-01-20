#ifndef __GOGAME_H__
#define __GOGAME_H__

#include "GoBoard.h"

class GoGame
{
public:
	GoGame(int boardSize);
	~GoGame();

	GoBoard* Board;

	GoPoint GenerateMove(int color);
	void Play(int color, int x, int y);
};

#endif //#ifndef __GOGAME_H__