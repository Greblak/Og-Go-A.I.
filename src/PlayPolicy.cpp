/*
 * PlayPolicy.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */
#include <vector>
#include "PlayPolicy.h"
#include "GoBoard.h"
#include "Log.h"

PlayPolicy::PlayPolicy()
{
  // TODO Auto-generated constructor stub

}

PlayPolicy::~PlayPolicy()
{
  // TODO Auto-generated destructor stub
}
const std::vector<int> PlayPolicy::FindPossibleLocalMoves(GoBoard* board)
{
  std::vector<int> moves;
  if(board->movePointer == 0) //No previous moves. No possible local answers
    return moves;
  int lastPlayed = board->Pos((board->moves[board->movePointer])->Point);
  //  std::cerr<<"LP"<<lastPlayed<<std::endl;
  int allLocalMoves[8];
  allLocalMoves[0] = (board->North(lastPlayed));
  allLocalMoves[1] = (board->West(lastPlayed));
  allLocalMoves[2] = (board->East(lastPlayed));
  allLocalMoves[3] = (board->South(lastPlayed));
  allLocalMoves[4] = (board->North(board->East(lastPlayed)));
  allLocalMoves[5] = (board->North(board->West(lastPlayed)));
  allLocalMoves[6] = (board->South(board->East(lastPlayed)));
  allLocalMoves[7] = (board->South(board->West(lastPlayed)));


  for(int i = 0; i < 8; ++i)
    {
      if(board->IsRealPoint(allLocalMoves[i]))
          {
          if(board->Occupied(allLocalMoves[i]) && board->State.blockPointers[allLocalMoves[i]]->Liberties() == 1)
            {
              int lib = board->State.blockPointers[allLocalMoves[i]]->LastLiberty();
              std::cerr<<"Found liberty at "<<lib<<std::endl;
              if(lib != -1 && (board->IsLegal(lib, S_BLACK) || board->IsLegal(lib, S_WHITE)))
                {
                  moves.push_back(lib);
                  std::cerr <<"Lib legal"<<std::endl;
                }

            }

          if(board->IsEmpty(allLocalMoves[i]) && board->IsLegal(allLocalMoves[i],board->NextPlayer())  )
            {

              if(MatchAny(board,allLocalMoves[i],board->NextPlayer()))
                moves.push_back(allLocalMoves[i]);
            }
          }
    }
  return moves;
}
const bool PlayPolicy::MatchAny(GoBoard* board, const int pos, const int color)
{
  int direction = board->POS_NS; //Init default value, to be safe :)
  for(int i = 0; i<4; ++i)
    {
      switch(i)
      {
      case(0):direction = board->POS_NS;
      break;
      case(1):direction = -board->POS_NS;
      break;
      case(2):direction = board->POS_WE;
      break;
      case(3):direction = -board->POS_WE;
      break;
      }

      if(TestAllHane(board,pos,color,direction) || TestAllCut(board,pos,color,direction)) // && TestAllCut
        return true;
    }
  return false;
}
const bool PlayPolicy::TestAllHane(GoBoard* board, const int pos, const int color, const int dirUp)
{
  return
		  TestHane1(board,pos,color,dirUp, getRightDirection(dirUp)) || TestHane1(board,pos,color,dirUp, getLeftDirection(dirUp))
		  || TestHane2(board,pos,color,dirUp, getRightDirection(dirUp)) || TestHane2(board,pos,color,dirUp, getLeftDirection(dirUp))
		  || TestHane3(board,pos,color,dirUp, getRightDirection(dirUp)) || TestHane3(board,pos,color,dirUp, getLeftDirection(dirUp))
		  || TestHane4(board,pos,color,dirUp, getRightDirection(dirUp)) || TestHane4(board,pos,color,dirUp, getLeftDirection(dirUp));
}

const bool PlayPolicy::TestAllCut(GoBoard* board, const int pos, const int color, const int dirUp)
{
  return TestCut1(board,pos,color,dirUp,getRightDirection(dirUp)) || TestCut1(board,pos,color,dirUp,getLeftDirection(dirUp))
		  || TestCut2(board,pos,color,dirUp,getRightDirection(dirUp)) || TestCut2(board,pos,color,dirUp,getLeftDirection(dirUp));
}
const bool PlayPolicy::TestHane1(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  if(
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos+dirUp+dirRight, color)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight))
    return true;

  if(
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos+dirUp-dirRight, opp)
      && board->IsColor(pos+dirUp+dirRight, opp)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight))
    return true;
  return false;
}
const bool PlayPolicy::TestHane2(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  if(
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsEmpty(pos+dirUp+dirRight)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight))
    return true;
  if(
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos+dirUp-dirRight, opp)
      && board->IsEmpty(pos+dirUp+dirRight)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight))
    return true;
  return false;
}

const bool PlayPolicy::TestHane3(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  if(
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos-dirRight, color)
      && board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirUp))
    return true;
  if(
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos+dirUp-dirRight, opp)
      && board->IsColor(pos-dirRight, opp)
      && board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirUp))
    return true;
  return false;
}

const bool PlayPolicy::TestHane4(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos+dirUp+dirRight, opp)
      && board->IsEmpty(pos-dirRight)
      && board->IsEmpty(pos+dirRight)
      && board->IsEmpty(pos-dirUp));
}

const bool PlayPolicy::TestCut1(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  if (
      ( //C1 hit
          board->IsColor(pos+dirUp, opp)
          && board->IsColor(pos+dirUp-dirRight, color)
          && board->IsColor(pos-dirRight, opp)
      )
      &&
      !( //Must not match below patterns
          //Atari 1
          board->IsColor(pos+dirRight, opp)
          //Atari 2
          || board->IsColor(pos-dirUp, opp)
      )
  )
    return true;
  if (
      ( //C1 hit
          board->IsColor(pos+dirUp, color)
          && board->IsColor(pos+dirUp-dirRight, opp)
          && board->IsColor(pos-dirRight, color)
      )
      &&
      !( //Must not match below patterns
          //Atari 1
          board->IsColor(pos+dirRight, color)
          //Atari 2
          || board->IsColor(pos-dirUp, color)
      )
  )
    return true;
  return false;
}

const bool PlayPolicy::TestCut2(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  this->board = board;
  int opp = getOpponentColor(color);
  return (
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos+dirRight, opp)
      && board->IsColor(pos-dirRight, opp)
      && (board->IsColor(pos-dirUp, color)          || board->IsEmpty(pos-dirUp))
      && (board->IsColor(pos-dirUp-dirRight, color) || board->IsEmpty(pos-dirUp-dirRight))
      && (board->IsColor(pos-dirUp+dirRight, color) || board->IsEmpty(pos-dirUp+dirRight))
  );
}

const bool PlayPolicy::TestEdge(GoBoard* board, const int pos, const int color, const int dirUp, const int dirRight)
{
  if(!board->IsBorder(pos))
    return false;
  this->board = board;
  int opp = getOpponentColor(color);

  //Situation 1
  if(
      board->IsColor(pos+dirUp-dirRight, color)
      && board->IsColor(pos-dirRight, opp)
      && board->IsEmpty(pos+dirUp)
  )
    return true;

  if(
      board->IsColor(pos+dirUp-dirRight, opp)
      && board->IsColor(pos-dirRight, color)
      && board->IsEmpty(pos+dirUp)
  )
    return true;

  //Situation 2
  if(
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos-dirRight, color)
      && board->IsColor(pos+dirRight, opp)
  )
    return true;

  if(
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos-dirRight, opp)
      && board->IsColor(pos+dirRight, color)
  )
    return true;

  //Situation 3
  if(
      board->IsColor(pos+dirUp, color)
      && board->IsColor(pos+dirUp+dirRight, opp)
  )
    return true;

  //Situation 4
  if(
      board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirUp+dirRight, color)
      && board->IsColor(pos+dirRight, opp)
  )
    return true;

  //Situation 5
  if(
      board->IsColor(pos-dirRight, color)
      && board->IsColor(pos+dirUp, opp)
      && board->IsColor(pos+dirRight, opp)
      && board->IsColor(pos+dirUp+dirRight, color)
  )
    return true;

  return false;
}


const int PlayPolicy::getRightDirection(const int dirUp) const
{
	std::cerr<<"Attempting to get right dir for "<<dirUp<<std::endl;
  if(dirUp == board->POS_NS)
    return board->POS_WE;
  if(dirUp == -board->POS_NS)
    return -board->POS_WE;
  if(dirUp == board->POS_WE)
    return -board->POS_NS;
  if(dirUp == -board->POS_WE)
    return board->POS_NS;
  throw "Unknown direction";
}

const int PlayPolicy::getLeftDirection(const int dirUp) const
{
 return -getRightDirection(dirUp);
}

const int PlayPolicy::getOpponentColor(int color) const
{
  return color == S_BLACK ? S_WHITE : S_BLACK;
}
