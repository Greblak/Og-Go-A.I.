#include "GoPoint.h"

GoPoint::GoPoint(void)
{

}

GoPoint::~GoPoint()
{

}

const bool GoPoint::IsPass()
{
  if(x==-1 && y==-1 && color == NONE)
    return true;
  return false;
}
