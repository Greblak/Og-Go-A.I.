#ifndef __GOPOINT_H__
#define __GOPOINT_H__

enum GoStoneColor
{
	S_BLACK, S_WHITE
};

enum GoBoardColor
{
	NONE, B_BLACK, B_WHITE
};

class GoPoint
{
public:
	GoPoint(void);
	virtual ~GoPoint();

private:

};

#endif //#ifndef __GOPOINT_H__