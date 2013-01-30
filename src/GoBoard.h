#ifndef __GOBOARD_H__
#define __GOBOARD_H__

#include "GoPoint.h"
#include "GoMove.h"
#include "Config.h"
#include "GoState.h"
#include "GoBlock.h"
#include <list>

typedef std::list<GoBlock*> BlockList;
typedef std::list<GoBlock*>::iterator BlockListIterator;

class GoBoard
{
public:
	GoBoard(int size = BOARD_DEFAULT_SIZE);
	virtual ~GoBoard();

		const int Size() const;
		const bool Occupied(const GoPoint p) const;
		const bool Occupied(const int p) const;
		const bool IsEmpty(GoPoint p) const;
		const bool IsBorder(int pos) const;
		const bool IsCorner(int pos) const;
		bool IsColor(GoPoint p, int c) const;
		static const int CurrentPlayer();
		int NextPlayer() const;

		int North(const GoPoint p) const;
		int South(const GoPoint p) const;
		int West(const GoPoint p) const;
		int East(const GoPoint p) const;
		int North(const int) const;
		int South(const int) const;
		int West(const int) const;
		int East(const int) const;
		int Pos(const GoPoint p) const;

		bool IsSuicide(const GoPoint p) const;
		bool IsRealPoint(const GoPoint p) const;

		bool IsLegal(const GoPoint& p, int color); 

		const bool IsLibertyOfBlock(const int point, const int anchor) const;
		const int FindCommonLiberties(const int point, const int anchor) const; //Used when joining a single stone to a block
		void UpdateBlocks(const GoPoint p);
		void KillDeadBlocks();

		const bool CapturedStones() const;
		const std::list<GoPoint> GetCapturedStones() const;

		//Finds number of liberties for all stones in the same group as p
		const int Liberties(const GoPoint p) const;
		const int Liberties(int pos) const;
		bool InAtari(GoPoint p) const;

		const GoMove* Play(GoPoint p, int color);
		const GoMove* Play(GoPoint p);

		void AddStone(int point, int color);
		void UpdateBlocks(int point, int color);

		void DisplayCurrentState() const;
		GoState State;
private:
	int BoardSize;
	BlockList blocks;
	
};




#endif //#ifndef __GOBOARD_H__