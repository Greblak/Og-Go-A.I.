#ifndef __GOTIMEHANDLER_H__
#define __GOTIMEHANDLER_H__
/*

  Handles time based on the Canadian Byoyomi time model
  Compatible with GTP version 2

*/
#define GO_TIME_NOT_ACTIVE -1
#define GO_TIME_PROCESSING_BUFFER 1

class GoTimeHandler
{
 private:
  int mainTime;
  int byoyomiTime;
  int byoyomiStones;
  bool isActive;
  
 public:
  inline GoTimeHandler():mainTime(0),byoyomiTime(0),byoyomiStones(0),isActive(false)
    {
    }
  inline GoTimeHandler(int main,int byo,int byostones):mainTime(main),byoyomiTime(byo),byoyomiStones(byostones),isActive(true)
    {
    }
  inline ~GoTimeHandler()
  {
  }

  inline void UpdateTime(int time, int stones)
  {
    isActive = true;
    if(stones == 0)
      {
	mainTime = time;
      }
    else
      {
	byoyomiTime = time;
      }
    byoyomiStones = stones;
  }

  inline int GetTimeRemainingForMove()
  {
    if(isActive)
      {
	if(byoyomiStones == 0)
	  return mainTime;
	return byoyomiTime;
      }
    return GO_TIME_NOT_ACTIVE;
  }
};

#endif //#ifndef  __GOTIMEHANDLER_H__