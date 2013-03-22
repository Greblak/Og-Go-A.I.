/*
 * MonteCarlo.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */
#include <time.h>
#include "MonteCarlo.h"
#include "Log.h"
#include "Exception.h"

MonteCarlo::MonteCarlo(int searchWidth, int searchDepth):searchDepth(searchDepth),searchWidth(searchWidth)
{
  LOG_VERBOSE << "Initialised MC";
}

MonteCarlo::~MonteCarlo()
{
}

GoPoint MonteCarlo::generateMove(int color, GoGame* game)
{
  LOG_VERBOSE << "Generating MC move for "<<color;
  GoBoard* nboard = game->Board->copyBoard();
  GoPoint bestMove = GoPoint(-1,-1,color);
  float bestWinRate = 0;
  for(int j = 0; j<searchWidth; ++j)
    {
      GoPoint toPlay = SimpleRandomAI().generateMove(color,game);
//           LOG_VERBOSE<< "Testing move x:"<<toPlay.x<<" y:"<<toPlay.y;
      int wins = 0;

      for (int i = 0; i<searchDepth; ++i)
        {
//          LOG_VERBOSE<<"Testing1"<<std::endl;

//          LOG_VERBOSE<<"Testing2"<<std::endl;
          nboard->Play(toPlay);
//          LOG_VERBOSE<<"Testing3"<<std::endl;
          try
          {
        		  std::cerr<<"Simulated "<<(i*j + j)<<" games"<<std::endl;
              SimpleRandomAI().simulateGame(nboard);
//              LOG_VERBOSE<<"Testing4"<<std::endl;
              float score = nboard ->GetScore();
              if(color == S_BLACK && score>0)
                ++wins;
              else if(color == S_WHITE && score<0)
                ++wins;

//              LOG_VERBOSE<<score<<std::endl;
          }catch(Exception& e)
          {
              LOG_DEBUG <<e.getMessage();
          }
//          nboard->DisplayCurrentState();
          nboard->resetAndReplayMoves(game->Board);

        } //Invert if black
      double winRate = (double)wins*1.0/(double)searchDepth;
      LOG_VERBOSE<<"Winrate "<<game->Board->ReadablePosition(toPlay)<<"("<<toPlay.x<<","<<toPlay.y<<") : "<<winRate<<std::endl;
      if(winRate > bestWinRate)
        {

          bestWinRate = winRate;
          bestMove.x = toPlay.x;
          bestMove.y = toPlay.y;

        }
//      LOG_VERBOSE<<"Time used to simulate games for one move:("<<toPlay.x<<","<<toPlay.y<<") "<<(float)(clock() - time)/CLOCKS_PER_SEC<<std::endl;
    }
  if(bestWinRate<0.4)
    {
      LOG_ERROR<<"Resign! The only winning move is not to play "<<std::endl;
    }
  LOG_VERBOSE<<"Best move ("<<bestMove.x<<","<<bestMove.y<<") : "<<bestWinRate<<std::endl;
  return bestMove;
}

