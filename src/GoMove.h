#ifndef __GOMOVE_H__
#define __GOMOVE_H__

#include "GoPoint.h"
#include "GoBlock.h"

class GoMove
{
public:
	GoMove(void);
	virtual ~GoMove();

private:
	GoStoneColor Color;
	GoPoint Point;
	GoBlock* Block;
};

#endif //#ifndef __GOMOVE_H__