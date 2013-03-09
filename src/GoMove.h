#ifndef __GOMOVE_H__
#define __GOMOVE_H__

#include "GoPoint.h"

class GoMove
{
public:
	GoMove(int color, GoPoint point);//, GoBlock* block);
	virtual ~GoMove();

	const int Color; ///< Color of the stone
	const GoPoint Point; ///< Position of the stone
};

#endif //#ifndef __GOMOVE_H__
