#ifndef __GOSTATE_H__
#define __GOSTATE_H__

struct GoState
{
	GoPoint koPoint;
	GoStoneColor toPlay;
	std::list<GoBlock*> blocks;
	int[2] bw_prisoners;
	int[2] bw_numstones;
	std::list<int> stones;

	bool isNewPosition;
};

#endif //#ifndef __GOSTATE_H__