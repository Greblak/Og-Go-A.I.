#ifndef __GOPOINT_H__
#define __GOPOINT_H__
#include "Config.h"

#define POINT_PASS GoPoint(-1,-1,NONE)
enum GoStoneColor
{
	S_BLACK = 0,
	S_WHITE
};

enum GoBoardColor
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
	const bool IsPass();
	bool operator==(GoPoint &a);
	int x;
	int y;
	int color;
private:

};

#endif //#ifndef __GOPOINT_H__
