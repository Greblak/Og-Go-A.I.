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
#include "GoBoard.h"

MonteCarlo::MonteCarlo(int moves, int simulations):numSimulations(simulations),numMoves(moves)
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
  for(int j = 0; j<numMoves; ++j)
    {
      GoPoint toPlay = SimpleRandomAI().generateMove(color,game);
      int wins = 0;
      for (int i = 0; i<numSimulations; ++i)
        {
          nboard->Play(toPlay);
          try
	    {
	      std::cerr<<"Simulated "<<(i*j + j)<<" games"<<std::endl;
              SimpleRandomAI().simulateGame(nboard);
              float score = nboard ->GetScore();
              if(color == S_BLACK && score>0)
                ++wins;
              else if(color == S_WHITE && score<0)
                ++wins;
	    }catch(Exception& e)
	    {
              LOG_DEBUG <<e.getMessage();
	    }
          nboard->resetAndReplayMoves(game->Board);
        } //Invert if black
      double winRate = (double)wins*1.0/(double)numSimulations;
      LOG_VERBOSE<<"Winrate "<<game->Board->ReadablePosition(toPlay)<<"("<<toPlay.x<<","<<toPlay.y<<") : "<<winRate<<std::endl;
      if(winRate > bestWinRate)
        {
          bestWinRate = winRate;
          bestMove.x = toPlay.x;
          bestMove.y = toPlay.y;
        }
    }
  if(bestWinRate<0.4)
    {
      LOG_ERROR<<"Resign! The only winning move is not to play "<<std::endl;
    }
  LOG_VERBOSE<<"Best move ("<<bestMove.x<<","<<bestMove.y<<") : "<<bestWinRate<<std::endl;
  return bestMove;
}

std::vector<UCBrow> MonteCarlo::generateMCTable(std::vector<int> moves, int color, GoGame* game)
{
  std::vector<UCBrow> ucbtable;
  GoBoard* nboard = game->Board->copyBoard();
  for(std::vector<int>::iterator it = moves.begin(); it != moves.end(); ++it)
    {
      int toPlay = (*it);
      float wins = 0;
      for (int i = 0; i<numSimulations/moves.size(); ++i)
        {
          nboard->Play(toPlay,color);
          try
	    {
	      SimpleRandomAI().simulateGame(nboard);
              float score = nboard ->GetScore();
              if(color == S_BLACK && score>0)
                ++wins;
              else if(color == S_WHITE && score<0)
                ++wins;
	    }
	  catch(Exception& e)
	    {
              LOG_DEBUG <<e.getMessage();
	    }
          nboard->resetAndReplayMoves(game->Board);
        } //Invert if black
      UCBrow u;
      u.pos = toPlay;
      u.expected = wins/numSimulations;
      u.timesPlayed = numSimulations; //Somewhat redundant, but reuses UCB table
      ucbtable.push_back(u);
    }
  return ucbtable;
}

