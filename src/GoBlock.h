#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoBoard.h"
#include "GoPoint.h"

#include <list>

class GoBlock
{
public:
	GoBlock();
	virtual ~GoBlock();
	int liberties;
	int color;
	int anchor;
	std::list<int> stones;
	
	const int Liberties() const;

	GoBoard* board;
};

#endif //#ifndef __GOBLOCK_H__

