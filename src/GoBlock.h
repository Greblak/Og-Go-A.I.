#ifndef __GOBLOCK_H__
#define __GOBLOCK_H__

#include "GoPoint.h"

class GoBlock
{
public:
	GoBlock(void);
	~GoBlock();
private:
	std::list<GoPoint*> stones;
};

#endif //#ifndef __GOBLOCK_H__
