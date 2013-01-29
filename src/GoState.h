#ifndef __GOSTATE_H__
#define __GOSTATE_H__

#include "GoState.h"
#include "GoPoint.h"

class GoBlock;
struct GoState
{
	GoPoint koPoint;
	GoStoneColor toPlay;
	// std::list<GoBlock*> blocks;
	int bw_prisoners[2];
	int bw_numstones[2];
	int stones[BOARD_MAX_SIZE*BOARD_MAX_SIZE];
	int numNeighbours[2][BOARD_MAX_SIZE*BOARD_MAX_SIZE]; //[Color][Pos]
	int numNeighboursEmpty[BOARD_MAX_SIZE*BOARD_MAX_SIZE]; 
	bool isNewPosition;
	GoBlock* blockPointers[BOARD_MAX_SIZE*BOARD_MAX_SIZE];
};

#endif //#ifndef __GOSTATE_H__