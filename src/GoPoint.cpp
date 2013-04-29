#include "GoPoint.h"

GoPoint::GoPoint(void)
{

}

GoPoint::~GoPoint()
{

}

const bool GoPoint::IsPass() const
{
  if(x==-1 && y==-1 && color == NONE)
    return true;
  return false;
}

bool GoPoint::operator==(GoPoint& a)
{
  if(a.x == x && a.y == y && a.color == color)
    return true;
  return false;
}

