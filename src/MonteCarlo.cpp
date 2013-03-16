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
  GoPoint bestMove = GoPoint(-1,-1,color);
  float bestWinRate = 0;
  for(int j = 0; j<searchWidth; ++j)
    {
      GoPoint toPlay = SimpleRandomAI().generateMove(color,game);
//           std::cerr<< "Testing move x:"<<toPlay.x<<" y:"<<toPlay.y;
      int wins = 0;

      for (int i = 0; i<searchDepth; ++i)
        {
//          std::cerr<<"Testing1"<<std::endl;
          GoBoard* nboard = game->Board->copyBoard();
//          std::cerr<<"Testing2"<<std::endl;
          nboard->Play(toPlay);
//          std::cerr<<"Testing3"<<std::endl;
          try
          {

              SimpleRandomAI().simulateGame(nboard);
//              std::cerr<<"Testing4"<<std::endl;
              float score = nboard ->GetScore();
              if(color == S_BLACK && score>0)
                ++wins;
              else if(color == S_WHITE && score<0)
                ++wins;

//              std::cerr<<score<<std::endl;
          }catch(Exception& e)
          {
              LOG_DEBUG <<e.getMessage();
          }
//          nboard->DisplayCurrentState();
          delete nboard;

        } //Invert if black
      double winRate = (double)wins*1.0/(double)searchDepth;
      std::cerr<<"Winrate "<<game->Board->ReadablePosition(toPlay)<<"("<<toPlay.x<<","<<toPlay.y<<") : "<<winRate<<std::endl;
      if(winRate > bestWinRate)
        {

          bestWinRate = winRate;
          bestMove.x = toPlay.x;
          bestMove.y = toPlay.y;

        }
//      std::cerr<<"Time used to simulate games for one move:("<<toPlay.x<<","<<toPlay.y<<") "<<(float)(clock() - time)/CLOCKS_PER_SEC<<std::endl;
    }
  if(bestWinRate<0.4)
    {
      LOG_ERROR<<"Resign! The only winning move is not to play "<<std::endl;
    }
  std::cerr<<"Best move ("<<bestMove.x<<","<<bestMove.y<<") : "<<bestWinRate<<std::endl;
  return bestMove;
}

