#include <sstream>
#include "Log.h"
#include "GoMove.h"
#include "GoGame.h"
#include "SGFEngine.h"
#include <cstdlib>
#include "Exception.h"
#include "SimpleRandomAI.h"
#include "MonteCarlo.h"
#include "UpperConfidence.h"
#include "EGTPEngine.h"
#include "GoTimeHandler.h"

GoGame::GoGame(int boardSize)
{

	Board = new GoBoard(boardSize);
}

GoGame::~GoGame()
{
	delete Board;
}

void GoGame::Play(int color, int x, int y)
{
	GoPoint p = GoPoint(x,y,color);
	Board->Play(p);

	//  SGFEngine::generateFile(this);
}

GoPoint GoGame::GenerateMove(int color)
{
//	  MonteCarlo ai(25,300);
//	GoPoint point = SimpleRandomAI().generateMove(color,this);
//	  UpperConfidence ai(10,3000);
//	LOG_DEBUG<<"Generating move for "<<color;
//	GoPoint point = ai.generateMove(color,this);
	//  if(point.x != -1 && point.y != -1 && point.color == NONE) //PASS
	//    Play(point.color, point.x, point.y);

//	GoPoint point = EGTPEngine::genMoveFromChildProcs(color,this);
    GoPoint point;
	return point;
}

