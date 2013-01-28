#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoBoard.h"
#include "GoPoint.h"

#include <list>

class GoBlock
{
public:
	GoBlock(GoBoard& b);
	virtual ~GoBlock();
	int Liberties() const;
private:
	GoStoneColor color;
	GoPoint anchor;
	std::list<GoPoint*> stones;

	GoBoard& board;
};

#endif //#ifndef __GOBLOCK_H__

