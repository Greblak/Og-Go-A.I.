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

		int North(GoPoint p) const;
		int South(GoPoint p) const;
		int West(GoPoint p) const;
		int East(GoPoint p) const;

		bool IsSuicide(GoPoint p) const;
		bool IsRealPoint(GoPoint p) const;

		bool IsLegal(GoPoint p, int color) const;

		const bool CapturedStones() const;
		const std::list<GoPoint> GetCapturedStones() const;

		int Liberties(GoPoint p) const;
		bool InAtari(GoPoint p) const;

		const GoMove* Play(GoPoint p, int color);
		const GoMove* Play(GoPoint p);
private:
	int BoardSize;
	GoState State;
};

#endif //#ifndef __GOBOARD_H__