#ifndef __GOSTATE_H__
#define __GOSTATE_H__

#include "GoState.h"
#include "GoPoint.h"
#include <list>
// #include "GoBlock.h"



struct GoState
{
	GoPoint koPoint;
	GoStoneColor toPlay;
	// std::list<GoBlock*> blocks;
	int bw_prisoners[2];
	int bw_numstones[2];
	int stones[BOARD_MAX_SIZE*BOARD_MAX_SIZE];
	int numNeighbours[BOARD_MAX_SIZE*BOARD_MAX_SIZE];
	bool isNewPosition;
};

#endif //#ifndef __GOSTATE_H__