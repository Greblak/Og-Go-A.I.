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

extern int LogLevel;
extern bool doTests;
extern bool keepRand;
extern int RAND_SEED;
GTPEngine::GTPEngine(void):commandNum(0)
{
  LOG_DEBUG<< "Starting GTP Engine";
  game = new GoGame(BOARD_DEFAULT_SIZE);
  if(doTests)
    {
      std::string line = "test";
      std::ifstream file;
      file.open("test.gtp");
      if(file.fail())
        {
          LOG_DEBUG << "Test file failed to open. Business as usual";
          return;
        }
      int i = 0;
      while(std::getline(file,line))
        {
          std::cout << line <<std::endl;
          parse(line);
          ++i;
        }
    }
}

GTPEngine::~GTPEngine()
{

}

void GTPEngine::parse(std::string userInput)
{
  LOG_VERBOSE << "Parsing GTP Command # "<<++commandNum;
  std::vector<std::string> args;
  boost::split(args, userInput,boost::is_any_of( " " ));

  std::vector<GoMove*> genmoves;

  if(args[0] == "name")
    LOG_OUT << "= "+PROGRAM_NAME;
  else if(args[0] == "version")
    LOG_OUT << "= "+PROGRAM_VERSION;
  else if(args[0] == "protocol_version")
    LOG_OUT << "= "+PROGRAM_GTP_VERSION;
  else if(args[0] == "list_commands")
    {
      LOG_OUT << "= name\nversion\nprotocol_version\nlist_commands\nboardsize\ngenmove\nplay\nclear_board\nshowboard\nquit\nfinal_score";
    }
  else if(args[0] == "boardsize")
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
      game = new GoGame(bsize);
      LOG_OUT << "= 1";
    }
  else if(args[0] == "clear_board")
    {
      int size = game->Board->Size();
      delete game;
      if(keepRand)
        srand (RAND_SEED);
      game = new GoGame(size);
      LOG_OUT << "= 1";
    }
  else if(args[0] == "play" || (LogLevel >= DEBUG && args[0] == "p"))
    {
      if(args[2] != "PASS")
        game->Play(ColorFromString(args[1]), ColumnStringToInt(args[2].substr(0,1)),
            RowStringToInt(args[2].substr(1,2)));
      LOG_OUT << "= 1";
    }
  else if(args[0] == "genmove")
    {
      GoPoint pos = game->GenerateMove(ColorFromString(args[1]));
      if(pos.x == -1 && pos.y == -1) // Play pass
        {
          LOG_OUT << "= PASS";
          return;
        }
      for(std::vector<GoMove*>::iterator it = genmoves.begin(); it != genmoves.end(); ++it)
        {
          if(pos.x == (*it)->Point.x && pos.y == (*it)->Point.y)
            {
              LOG_ERROR<<"ERROR IN MOVE GENERATION. MOVE HAS BEEN PLAYED";
              throw "FUCK THIS I'm OUT";
            }
        }
      LOG_VERBOSE << "Generated move at "<<pos.x<<","<<pos.y;
      genmoves.push_back(new GoMove(ColorFromString(args[1]),pos));
      LOG_OUT <<"= " << ColumnIntToString(pos.x)<<RowIntToString(pos.y);
    }
  else if(args[0] == "showboard")
    {
      game->Board->DisplayCurrentState();
    }

  else if(args[0] == "final_score")
    {
      int i = system("gnugo --score estimate --quiet -l SGF.sgf > score.s");
      std::ifstream t;
      t.open("score.s");
      if(t.fail())
        throw Exception("Unable to score game");
      std::string line;
      std::getline(t, line);
      t.close();
      std::vector<std::string> a;
      boost::split(a, line,boost::is_any_of( " " ));
      char color = a[0] == "White" ? 'W':'B';
      float score = atoi(a[3].c_str());
      LOG_OUT << "= "<<color<<"+"<<score;



    }
  else if(args[0] == "quit")
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
    throw Exception("Unknown command");

  //  game->Board->DisplayCurrentState();
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
  int n = (str[0] - GTP_OFFSET_NUM);
  if(n>9)
    throw Exception("Invalid row input");
  if(str[1] != 0)
    {
      n*=10;
      n+= str[1] - GTP_OFFSET_NUM;
    }
  n--;

  if(n < 0 || n >= BOARD_MAX_SIZE)
    throw Exception("Invalid row number");

  return n;
}

const int GTPEngine::ColorFromString(std::string str) const
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
