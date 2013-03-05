#ifndef __GOMOVE_H__
#define __GOMOVE_H__

#include "GoPoint.h"

class GoMove
{
public:
	GoMove(int color, GoPoint point);//, GoBlock* block);
	virtual ~GoMove();

	const int Color;
	const GoPoint Point;
};

#endif //#ifndef __GOMOVE_H__
