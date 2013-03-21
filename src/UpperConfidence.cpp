/*
 * UpperConfidence.cpp
 *
 *  Created on: Mar 16, 2013
 *      Author: rune
 */
#include <math.h>
#include "UpperConfidence.h"
#include "Log.h"
#include "GoBoard.h"
#include "PlayPolicy.h"
#include "SimpleRandomAI.h"
UpperConfidence::UpperConfidence(int searchWidth, int searchDepth):searchDepth(searchDepth),searchWidth(searchWidth),expRatio(1.5)
{

}

UpperConfidence::~UpperConfidence()
{
	delete workingBoard;
}

GoPoint UpperConfidence::generateMove(int color, GoGame* game)
{
	workingBoard = new GoBoard(game->Board->Size());
  std::cerr<<"SD: "<<searchDepth<<std::endl;
  this->color = color;
  std::cerr<<"SD: "<<color<<std::endl;
  this->game = game;
  std::cerr<<"SD: "<<game<<std::endl;
  std::cerr << "Generating UCB move for "<<color<<std::endl;
  //Generate possible moves
  std::cerr << "Generating move list"<<std::endl;
  std::vector<int> moves = PlayPolicy().FindPossibleLocalMoves(game->Board);
  std::cerr<< "Found "<<moves.size()<< " possible local moves"<<std::endl;
  //Add other moves not covered by plays
  if(moves.size()==0)
    {
      for(int i = 0; i<20;++i)
        moves.push_back(game->Board->Pos(SimpleRandomAI().generateMove(color,game)));
    }
  float expected[moves.size()];
  int numPlayed[moves.size()];
  int totalNumPlayed = 0;

  int initialPlayNum = 1;
  LOG_VERBOSE << "Play all 1 time";
  //Play all moves "once"
  for(size_t j = 0; j<moves.size(); ++j)
    {
      numPlayed[j] = 0;
      expected[j] = 0;
      for(int i = 0; i<initialPlayNum; ++i)
        {
          float result = simulateMove(moves[j]);
          if(result > 0)
            result = 1;
          else
            result = 0;

          float oldWins = expected[j] * numPlayed[j];
          ++numPlayed[j];
          ++totalNumPlayed;
          expected[j] = ((float)(result+oldWins)/(float)numPlayed[j]);

        }
    }

  float maximisedVal = 0.0;
  int nextToPlay = 0;
  while(totalNumPlayed<searchDepth)
    {
      //Maximise for all following plays
      for(size_t i = 0; i<moves.size(); ++i)
        {
          float ucbVal = expected[i] + sqrt( expRatio * log(totalNumPlayed) / numPlayed[i]);
          //          std::cerr <<i<< " " <<expected[i]<< " "<< sqrt( expRatio * log(totalNumPlayed) / numPlayed[i]) << std::endl;
          if(ucbVal > maximisedVal)
            {
              maximisedVal = ucbVal;
              nextToPlay = i;
            }
        }
      //      std::cerr <<nextToPlay<< " " <<expected[nextToPlay]<< " "<< sqrt( expRatio * log(totalNumPlayed) / numPlayed[nextToPlay]) << std::endl;
      //Play best move and update expected return
      float result = simulateMove(moves[nextToPlay]);
      if(result > 0)
        result = 1;
      else
        result = 0;
      ++numPlayed[nextToPlay];
      ++totalNumPlayed;
      if(totalNumPlayed%100==0)
        std::cerr<<"Simulated "<<totalNumPlayed<<" games"<<std::endl;

      float oldWins = expected[nextToPlay] * numPlayed[nextToPlay];
      ++numPlayed[nextToPlay];
      expected[nextToPlay] = ((float)(result+oldWins)/(float)numPlayed[nextToPlay]);

      maximisedVal = 0;
      nextToPlay = 0;
    }

  //Select best move after search completion
  int bestMove = -1;
  float bestExpected = 0;
  for(size_t i = 0; i<moves.size(); ++i)
    {
      std::cerr<<game->Board->ReadablePosition(moves[i])<<" E:"<<expected[i]<<" Played:"<<numPlayed[i]<<std::endl;
      if(expected[i]>bestExpected)
        {
          std::cerr<<"BE is now"<<game->Board->ReadablePosition(moves[i])<<std::endl;
          bestExpected = expected[i];
          bestMove = i;
        }
    }

  LOG_DEBUG<<"Best move ("<<game->Board->ReadablePosition(moves[bestMove])<<") : "<<bestExpected<<std::endl;
  return game->Board->ReversePos(moves[bestMove],color);
}

const float UpperConfidence::simulateMove(int move)
{
  float expected = 0;
  workingBoard->resetAndReplayMoves(game->Board);
  workingBoard->Play(move, workingBoard->NextPlayer());
  SimpleRandomAI().simulateGame(workingBoard);
  float score = workingBoard->GetScore();
  if(color == S_BLACK && score>0)
    expected += score;
  else if(color == S_WHITE && score<0)
    expected -= score;
  return expected;
}
