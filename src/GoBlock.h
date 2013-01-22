#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoPoint.h"
#include "GoBoard.h"
#include <list>

class GoBlock
{
public:
	inline GoBlock(const GoBoard& board):board(board){

	}
	virtual ~GoBlock();
	int Liberties() const;
private:
	GoStoneColor color;
	GoPoint anchor;
	std::list<GoPoint*> stones;
	const GoBoard& board;
};

#endif //#ifndef __GOBLOCK_H__

