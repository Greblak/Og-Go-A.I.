#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoBoard.h"
#include "GoPoint.h"

#include <list>

const int BLOCK_MAX_STONES = BOARD_MAX_SIZE*BOARD_MAX_SIZE-1;

class GoBoard;
class GoBlock
{
public:
	GoBlock();
	virtual ~GoBlock();
	int liberties; ///< Number of liberties remaining
	int color; ///< Stone color
	int anchor; ///< First stone to create the group
	int lastStone; 
	int stones[BLOCK_MAX_STONES]; ///< List of stone positions connected by the block
	
	const int Liberties() const; ///< Number of liberties remaining for the block
	void ImportBlock(GoBlock* block); ///< Import an entire block into this block, update liberties and add stones. Deletes the other block
	void RemoveStones(); ///< Deletes all stones from a block.
	const int LastLiberty() const; ///< Last liberty of the block
	void addStone(int pos); ///< Add a stone to the block
	void reset(); ///< Reset the block to starting conditions.

	GoBoard* board; ///< A pointer is held to allow access to manipulative board functions
};

#endif //#ifndef __GOBLOCK_H__

