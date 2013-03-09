#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoBoard.h"
#include "GoPoint.h"

#include <list>

class GoBoard;
class GoBlock
{
public:
	GoBlock();
	virtual ~GoBlock();
	int liberties; ///< Number of liberties remaining
	int color; ///< Stone color
	int anchor; ///< First stone to create the group
	std::list<int> stones; ///< List of stone positions connected by the block
	
	const int Liberties() const;
	void ImportBlock(GoBlock* block); ///< Import an entire block into this block, update liberties and add stones. Deletes the other block
	void RemoveStones(); ///< Deletes all stones from a block.

	GoBoard* board; ///< A pointer is held to allow access to manipulative board functions
};

#endif //#ifndef __GOBLOCK_H__

