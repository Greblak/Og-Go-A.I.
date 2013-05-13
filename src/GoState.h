#ifndef __GOSTATE_H__
#define __GOSTATE_H__

#include "GoState.h"
#include "GoPoint.h"

class GoBlock;
struct GoState
{
  int koPoint;
  int toPlay; ///< Color of active player
  int bw_prisoners[2]; ///< Number of stones captured by black and white
  int stones[BOARD_MAX_SIZE*BOARD_MAX_SIZE]; ///< Where all stones and their colors are stored.
  //Number of neighbours of a specific color[arg 1]
  int numNeighbours[2][BOARD_MAX_SIZE*BOARD_MAX_SIZE]; ///<Number of neighbours for of a given color. usage: numNeighbours[black/white enum][pos]
  int numNeighboursEmpty[BOARD_MAX_SIZE*BOARD_MAX_SIZE]; ///<Number of empty points around a position
  bool isNewPosition; ///< Used to determine if ko is needed
  GoBlock* blockPointers[BOARD_MAX_SIZE*BOARD_MAX_SIZE]; ///< Pointers to all blocks. To find the block of a stone at position pos, use blockPointers[pos]
};

#endif //#ifndef __GOSTATE_H__
