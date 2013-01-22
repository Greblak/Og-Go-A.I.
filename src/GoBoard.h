#ifndef __GOBOARD_H__
#define __GOBOARD_H__

#include "GoPoint.h"
#include "GoMove.h"
#include "GoState.h"
#include "Config.h"
#include <list>

class GoBoard
{
public:
	GoBoard(int size = BOARD_DEFAULT_SIZE);
	virtual ~GoBoard();

		const int Size() const;
		bool Occupied(GoPoint p) const;
		bool IsEmpty(GoPoint p) const;
		bool IsBorder(GoPoint p) const;
		bool IsColor(GoPoint p, int c) const;
		static const int CurrentPlayer();
		int NextPlayer() const;

		int North(const GoPoint p) const;
		int South(const GoPoint p) const;
		int West(const GoPoint p) const;
		int East(const GoPoint p) const;
		int Pos(const GoPoint p) const;

		bool IsSuicide(const GoPoint p) const;
		bool IsRealPoint(const GoPoint p) const;

		bool IsLegal(const GoPoint& p, int color); 

		const bool CapturedStones() const;
		const std::list<GoPoint> GetCapturedStones() const;

		//Finds number of liberties for all stones in the same group as p
		const int Liberties(const GoPoint p) const;
		bool InAtari(GoPoint p) const;

		const GoMove* Play(GoPoint p, int color);
		const GoMove* Play(GoPoint p);

		void DisplayCurrentState() const;
private:
	int BoardSize;
	GoState State;
};

#endif //#ifndef __GOBOARD_H__