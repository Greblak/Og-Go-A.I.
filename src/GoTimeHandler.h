#ifndef __GOTIMEHANDLER_H__
#define __GOTIMEHANDLER_H__
/*

  Handles time based on the Canadian Byoyomi time model
  Compatible with GTP version 2

*/
#define GO_TIME_NOT_ACTIVE -1 ///< Constant used to test if timer is active
#define GO_TIME_PROCESSING_BUFFER_DEFAULT 5 ///< Default processing buffer

class GoTimeHandler
{
  
 public:

  int GO_TIME_PROCESSING_BUFFER; ///< Directly editable processing buffer. Currently not used
  inline GoTimeHandler():mainTime(0),byoyomiTime(0),byoyomiStones(0),isActive(false)
    {
      GO_TIME_PROCESSING_BUFFER = GO_TIME_PROCESSING_BUFFER_DEFAULT;
    }
  inline GoTimeHandler(int main,int byo,int byostones):mainTime(main),byoyomiTime(byo),byoyomiStones(byostones),isActive(true)
    {
      GO_TIME_PROCESSING_BUFFER = GO_TIME_PROCESSING_BUFFER_DEFAULT;
    }
  inline ~GoTimeHandler()
  {
  }

  inline void UpdateTime(int time, int stones) ///<Updates time settings
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

  inline int GetTimeRemainingForMove() ///< Get remaining time for the current move
  {
    if(isActive)
      {
	if(byoyomiStones == 0)
	  return mainTime;
	return byoyomiTime;
      }
    return GO_TIME_NOT_ACTIVE;
  }

  inline bool IsActive()
  {
    return isActive;
  }
 private:
  int mainTime;
  int byoyomiTime;
  int byoyomiStones;
  bool isActive;
};

#undef GO_TIME_PROCESSING_BUFFER_DEFAULT

#endif //#ifndef  __GOTIMEHANDLER_H__
