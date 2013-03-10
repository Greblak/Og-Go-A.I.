/*
 * MonteCarlo.cpp
 *
 *  Created on: Mar 10, 2013
 *      Author: rune
 */
#include <time.h>
#include "MonteCarlo.h"
#include "Log.h"


MonteCarlo::MonteCarlo()
{
  LOG_VERBOSE << "Initialised MC";
  // TODO Auto-generated constructor stub

}

MonteCarlo::~MonteCarlo()
{
  // TODO Auto-generated destructor stub
}

GoPoint MonteCarlo::generateMove(int color, GoGame* game)
{
  LOG_VERBOSE << "Generating MC move";
  GoPoint point;

  GoBoard* rootBoard = game->Board;
  point = SimpleRandomAI().generateMove(color,game);
  GoBoard* nboard = copyBoard(game->Board, game->moves);
  delete nboard;



  return point;
}

GoBoard* MonteCarlo::copyBoard(GoBoard* old, std::vector<GoMove*>& moves)
{
  LOG_VERBOSE << "Copying board";
  clock_t t = clock();
  GoBoard* nBoard = new GoBoard(old->Size());

  for(std::vector<GoMove*>::const_iterator it; it != moves.end(); ++it)
    {
      nBoard->Play((*it)->Point);
    }
  t = clock() - t;
  LOG_VERBOSE << "Board copy ready. Completed in : "<<((float)t*(float)1000/CLOCKS_PER_SEC);
  return nBoard;
}

