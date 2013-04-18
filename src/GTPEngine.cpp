#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
#include "GTPEngine.h"
#include "Log.h"
#include "GoPoint.h"
#include "Config.h"
#include "GoGame.h"
#include "Exception.h"
//#include "Tests.h"

extern int LogLevel;
extern bool doTests;
extern bool keepRand;
extern int RAND_SEED;
GTPEngine::GTPEngine(void):commandNum(0)
{
  LOG_DEBUG<< "Starting GTP Engine";
  game = new GoGame(BOARD_DEFAULT_SIZE);

}

GTPEngine::~GTPEngine()
{

}

std::vector<std::string> GTPEngine::parse(std::string userInput)
{
  LOG_VERBOSE<< "Parsing GTP Command # "<<++commandNum<<" "<<userInput;
  std::vector<std::string> args;
  boost::split(args, userInput,boost::is_any_of( " " ));

  std::vector<GoMove*> genmoves;

  if(args[0] == GTP_CMD_NAME)
    LOG_OUT << "= "+PROGRAM_NAME;
  else if(args[0] == GTP_CMD_VERSION)
    LOG_OUT << "= "+PROGRAM_VERSION;
  else if(args[0] == GTP_CMD_PROTOC_VERS)
    LOG_OUT << "= "+PROGRAM_GTP_VERSION;
  else if(args[0] == GTP_CMD_LIST_CMDS)
    {
      LOG_OUT << "= name\nversion\nprotocol_version\nlist_commands\nboardsize\ngenmove\nplay\nclear_board\nshowboard\nquit\nfinal_score";
    }
  else if(args[0] == GTP_CMD_BOARDSIZE)
    {
      int bsize;
      try
	{
	  bsize = atoi(args[1].c_str());
	}
      catch( char * str)
	{
	  throw Exception("Invalid boardsize. NaN.");;
	}
      LOG_VERBOSE << "Attempting to set boardsize to "<<bsize;
      if(game != 0)
	delete game;
      game = new GoGame(bsize);
      LOG_OUT << "= 1";
    }
  else if(args[0] == GTP_CMD_CLEAR_BOARD)
    {
      int size = BOARD_DEFAULT_SIZE;
      if(game != 0)
	{
	  int size = game->Board->Size();
	  delete game;
	}
      if(keepRand)
	srand (RAND_SEED);
      game = new GoGame(size);
      LOG_OUT << "= 1";
    }
  else if(args[0] == GTP_CMD_PLAY || (LogLevel >= DEBUG && args[0] == "p"))
    {
      if(args[2] != "PASS")
	game->Play(ColorFromString(args[1]), ColumnStringToInt(args[2].substr(0,1)), RowStringToInt(args[2].substr(1,2)));
      LOG_OUT << "= 1";
    }
  else if(args[0] == GTP_CMD_GENMOVE)
    {
      LOG_VERBOSE<<"Genmove cmd"<<std::endl;
      return args;
    }
  else if(args[0] == GTP_CMD_SHOW_BOARD)
    {
      game->Board->DisplayCurrentState();
    }

  else if(args[0] == GTP_CMD_FINAL_SCORE)
    {
      float score = game->Board->GetScore();
      char color;
      if(score > 0)
	color = 'B';
      else
	{
	  color = 'W';
	  score*=-1;
	}
      LOG_OUT << "= "<<color<<"+"<<score;
    }
  else if(args[0] == GTP_CMD_QUIT)
    {
      exit(EXIT_SUCCESS);
    }
#ifdef DEBUG_MODE
  else if(args[0] == "d")
    {
      if(args[1] == "libstone")
	{
	  GoPoint p = GoPoint(ColumnStringToInt(args[2].substr(0,1)),RowStringToInt(args[2].substr(1,2)),B_WHITE);
	  LOG_DEBUG << "= "<<game->Board->State.numNeighboursEmpty[game->Board->Pos(p)]<<"\n\n";
	}
      else if(args[1] == "libblock")
	{
	  GoPoint p = GoPoint(ColumnStringToInt(args[2].substr(0,1)),RowStringToInt(args[2].substr(1,2)),B_WHITE);
	  LOG_DEBUG << "= "<<game->Board->State.blockPointers[game->Board->Pos(p)]->Liberties()<<"\n\n";
	}

      else if(args[1] == "eye")
	{
	  GoPoint p = GoPoint(ColumnStringToInt(args[2].substr(0,1)),RowStringToInt(args[2].substr(1,2)),B_WHITE);
	  int boardColor = ColorFromString(args[3]) +1;
	  if(game->Board->IsTrueEye(game->Board->Pos(p), boardColor))
	    LOG_DEBUG << "= "<<args[2]<<" is a true eye\n\n";
	  else
	    LOG_DEBUG << "= EEEEEEERRRR";
	}

      else if(args[1] == "rseed")
	{
	  RAND_SEED = std::atoi(args[2].c_str());
	}
      LOG_OUT << "= 1";
    }
#endif //IFDEF DEBUG_MODE
  else
    {
      std::stringstream ss;
      ss<<"Unknown command: \""<<args[1]<<"\""<<std::endl;
      throw Exception(ss.str());
    }

  //  game->Board->DisplayCurrentState();

  return args;
}

const int GTPEngine::ColumnStringToInt(std::string str) const
{
  int n = str[0];

  if(n >= GTP_OFFSET_LOWERCASE_A)
    n = n - GTP_OFFSET_LOWERCASE_A;
  else
    n = n - GTP_OFFSET_UPPERCASE_A;

  if(n < 0 || n > BOARD_MAX_SIZE || n == GTP_OFFSET_I)
    {
      throw Exception("Invalid column.");
    }

  if(n>= GTP_OFFSET_I)
    n--;

  return n;
}

const int GTPEngine::RowStringToInt(std::string str) const
{
  int i = 0;
  if(str[i] >= GTP_OFFSET_NUM+10) //Column first. use next
    ++i;
  int n = (str[i] - GTP_OFFSET_NUM);
  if(n>game->Board->Size())
    throw Exception("Invalid row input");
  if(str[i+1] != 0)
    {
      n*=10;
      n+= str[i+1] - GTP_OFFSET_NUM;
    }
  n--;

  if(n < 0 || n >= BOARD_MAX_SIZE)
    throw Exception("Invalid row number");
  
  return n;
}

int GTPEngine::ColorFromString(std::string str) const
{
  if(str == "w" || str == "W" || str == "white" || str == "WHITE")
    return S_WHITE;
  else if(str == "b" || str == "B" || str == "black" || str == "BLACK")
    return S_BLACK;
  else
    throw Exception("Unable to convert string to BoardColor.");
}

const char GTPEngine::ColumnIntToString(int n) 
{
  if(n>=GTP_OFFSET_I)
    n++;
  return n+GTP_OFFSET_UPPERCASE_A;
}

const int GTPEngine::RowIntToString(int n) 
{
  return ++n; //To account for 0-indexed to 1-indexed
}

const std::string GTPEngine::generateGTPString(GoBoard* board)
{
  std::stringstream ss;
  ss<<GTP_CMD_CLEAR_BOARD<< "\n";;
  ss<<GTP_CMD_BOARDSIZE<<" "<<board->Size()<<"\n";
  for(int i = 0; i<board->movePointer; ++i)
    {
      ss << "play ";
      if(board->moves[i]->Color == S_BLACK)
	ss<<"b ";
      else if(board->moves[i]->Color == S_WHITE)
	ss<<"w ";

      ss<<board->ReadablePosition(board->moves[i]->Point)<<"\n";
    }
  return ss.str();
}
