#ifndef __GOMOVE_H__
#define __GOMOVE_H__


 #include "GoPoint.h"


class GoMove
{
public:
	GoMove(int color, GoPoint* point, GoPoint* p);//, GoBlock* block);
	// GoMove::GoMove(int color, GoPoint* point, GoBlock* block):Color(color),Point(point),Block(block)
	virtual ~GoMove();

	const int Color;
	const GoPoint* Point;
	// const GoBlock* Block;
};

#endif //#ifndef __GOMOVE_H__