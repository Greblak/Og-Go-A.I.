#ifndef __GOBOARD_H__
#define __GOBOARD_H__

#include "GoPoint.h"
#include "GoMove.h"
#include "Config.h"
#include "GoState.h"
#include "GoBlock.h"
#include "GoMove.h"
#include <list>
#include <vector>

typedef std::list<GoBlock*> BlockList;
typedef std::list<GoBlock*>::iterator BlockListIterator;

class GoBoard
{
public:
	GoBoard(int size = BOARD_DEFAULT_SIZE);
	virtual ~GoBoard();

		const int Size() const; ///< Returns board size. Ex: 19x19 board will return 19
		const bool Occupied(const GoPoint p) const; ///< Returns true if p is occupied on the board
		const bool Occupied(const int p) const; ///< Returns true if p is occupied on the board
		const bool IsEmpty(const int pos) const; ///< Returns !Occupied(p)
		const bool IsEmpty(GoPoint p) const; ///< Returns !Occupied(p)
		const bool IsBorder(int pos) const; ///< Checks if pos is a border position on the board. Used to count liberties
		const bool IsCorner(int pos) const; ///< Checks if pos is a corner position on the board. Used to count liberties
		const bool IsColor(int pos, int color) const; ///< Checks a point for a given color
		const int CurrentPlayer() const; ///< Returns the current player color
		const int NextPlayer() const; ///< Inverted CurrentPlayer

		int North(const GoPoint p) const; ///< Returns the position north of p
		int South(const GoPoint p) const; ///< Returns the position south of p
		int West(const GoPoint p) const; ///< Returns the position west of p
		int East(const GoPoint p) const; ///< Returns the position east of p
		int North(const int) const; ///< Returns the position north of p
		int South(const int) const; ///< Returns the position south of p
		int West(const int) const; ///< Returns the position west of p
		int East(const int) const; ///< Returns the position east of p
		int Pos(const GoPoint p) const; ///< Returns the numerical position in the 1D array of point p

		bool IsSuicide(const GoPoint p) const; ///< Checks if playing on point p is a suicide
		bool IsSuicide(const int p, const int color) const; ///< Checks if playing on point p is a suicide
		bool IsRealPoint(const GoPoint p) const; ///< Checks point validity
		bool IsRealPoint(const int p) const; ///< Checks point validity

		bool IsLegal(const GoPoint& p, int color); ///< Checks if playing p is legal for color
		bool IsLegal(const int p, int color); ///< Checks if playing p is legal for color

		const bool IsLibertyOfBlock(const int point, const int anchor) const; ///< Checks if a given point is a liberty for the block the anchor point belongs to.
		const int FindCommonLiberties(const int point, const int anchor) const; ///<Used when joining a single stone to a block
		const int FindUniqueLiberties(const int stone, const GoBlock* block) const; ///< Counts unique liberties for stone compared to block
		void UpdateBlocks(const GoPoint p); ///< Update block liberties, stones and do cleanup of dead blocks
		void KillSurroundingDeadBlocks(const int p); ///< Checks all blocks surrounding p, remove them if liberties == 0
		const bool IsInSameBlock(const int pos1,const int pos2) const; //Compares positions to see if they share block
		const bool IsTrueEye(const int pos, const int boardColor);

		const bool CapturedStones() const; ///< Returns captured stones for this round
		const std::list<GoPoint> GetCapturedStones() const; ///< Returns a list of captured stones


		const int Liberties(const GoPoint p) const; ///< Finds number of liberties for all stones in the same group as p
		const int Liberties(int pos) const; ///< Finds number of liberties for all stones in the same group as p
		bool InAtari(GoPoint p) const; ///< Checks if point p is in atari(1 liberty)

		const GoMove* Play(int p, int color); ///< Attempt to play p with given color
		const GoMove* Play(GoPoint p, int color); ///< Attempt to play p with given color
		const GoMove* Play(GoPoint p); ///< Attempt to play p. Color is stored in p.

		void AddStone(int point, int color); ///< Add stone with color to board placed on point
		void RemoveStone(const int pos); ///< Remove stone at pos from the board
		void UpdateBlocks(int point, int color); ///< Update blocks based on placing a stone with the given parameters

		void DisplayCurrentState() const; ///< Used by GTP showboard command to output the current state to console
		const float GetScore() const; ///< Returns score. Positive score = black win. Negative score = white win
		const float GetScoreGnuGo() const; ///< Returns the score using an external program(GnuGo)
		const float GetScoreInternal() const; ///< Internal scoring method
		GoPoint ReversePos(const int pos, const int color); ///< Converts an integer position to a GoPoint
		const std::string ReadablePosition(const int pos) const; ///< Converts an integer position to a readable position in GTP format
		const std::string ReadablePosition(const GoPoint& pos) const; ///< Converts a GoPoint to a readable position in GTP format.

		GoBoard* copyBoard() const; ///< Copies the current board to a new heap object. This is done by creating a new board and replaying the moves
		void resetAndReplayMoves(GoBoard* board); ///< Resets the board and replays the board according to the moves done in the argument board.
		GoState State; ///< Struct containing the most basic board information
		int blockPointer; ///< Pointer to last active block index in the blocks-member
		int movePointer; ///< Pointer to last active move index in the moves-member
		GoBlock* blocks[1000]; ///< List of active blocks on the board
		GoMove* moves[1000]; ///< A list of moves made in this game. Used to reproduce in SGF files and copy game instances
		float komi; ///< Komi-score
		const int POS_WE; ///< Constant for west-east movement
		const int POS_NS; ///< constant for north-south movement
		void reset(); ///< Reset the board to a fresh start.
		
private:
	int BoardSize; ///< Boardsize (19x19 will be stored as 19 here)

	int BOARD_TOP_LEFT; ///< Location of top left point
	int BOARD_TOP_RIGHT; ///< Location of top right point
	int BOARD_BOTTOM_LEFT; ///< Location of bottom left point
	int BOARD_BOTTOM_RIGHT; ///< Location of bottom right point

	int nextPlayer; ///< Color of next player. Of enum type GoStoneColor

	inline GoBoard& operator=(const GoBoard& g) { }; ///< Prevents reassignment of stack object

};
#endif //#ifndef __GOBOARD_H__
