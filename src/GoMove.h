#ifndef __GOMOVE_H__
#define __GOMOVE_H__

#include "GoPoint.h"
#include "GoBlock.h"

class GoMove
{
public:
	GoMove(int color, GoPoint* point, GoBlock* block);
	virtual ~GoMove();

	const int Color;
	const GoPoint* Point;
	const GoBlock* Block;
};

#endif //#ifndef __GOMOVE_H__