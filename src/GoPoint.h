#ifndef __GOPOINT_H__
#define __GOPOINT_H__
#include "Config.h"

#define POINT_PASS GoPoint(-1,-1,NONE)
enum GoStoneColor ///< Used as color specifier when there must exist a color. For example color of stone or player
{
	S_BLACK = 0,
	S_WHITE
};

enum GoBoardColor ///< Used to define the color of a point. Used where there can be a point with no colors
{
	NONE = 0,
	B_BLACK,
	B_WHITE
};

class GoPoint
{
public:
	GoPoint(void);
	inline GoPoint(int x, int y, int color):x(x),y(y),color(color){}
	virtual ~GoPoint();
	const bool IsPass() const; ///< Tests if the point is a pass move. This is true if y=1, y=1, color=NONE
	bool operator==(GoPoint &a);
	int x; ///< x position in int value
	int y; ///< y position in int value
	int color; ///< color in GoBoardColor value
};

#endif //#ifndef __GOPOINT_H__
