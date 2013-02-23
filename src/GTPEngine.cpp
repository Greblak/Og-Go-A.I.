#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "GTPEngine.h"
#include "Log.h"
#include "GoPoint.h"
#include "Config.h"
#include "GoGame.h"
#include "Exception.h"

extern bool doTests;
GTPEngine::GTPEngine(void)
{
  game = new GoGame(BOARD_DEFAULT_SIZE);
  if(doTests)
    {
      Log::Deb("Setting up block join testbed. Play at C3",__FILE__,__LINE__);
      game->Play(S_WHITE,0,0);
      game->Play(S_WHITE,1,0);
      game->Play(S_WHITE,2,0);
      game->Play(S_WHITE,2,1);
      game->Play(S_WHITE,0,2);
      game->Play(S_WHITE,1,2);
    }
}

GTPEngine::~GTPEngine()
{

}

void GTPEngine::parse()
{
  std::string userInput;
  std::getline(std::cin, userInput);
  Log::In(userInput);

  std::vector<std::string> args;
  boost::split(args, userInput,boost::is_any_of( " " ));

  if(args[0] == "name")
    Log::Out("= "+PROGRAM_NAME);
  else if(args[0] == "version")
    Log::Out("= "+PROGRAM_VERSION);
  else if(args[0] == "protocol_version")
    Log::Out("= "+PROGRAM_GTP_VERSION);
  else if(args[0] == "list_commands")
    {
      Log::Out("= name\nversion\nprotocol_version\nlist_commands\nboardsize\ngenmove\nplay\nclear_board\nshowboard");
    }
  else if(args[0] == "boardsize")
    {
      if(game != NULL && game->Board->Size() != atoi(args[1].c_str()))
        throw Exception("Boardsize already set");

      int bsize;
      try
      {
          bsize = atoi(args[1].c_str());
      }
      catch( char * str)
      {
          throw Exception("Invalid boardsize. NaN.");;
      }

      game = new GoGame(bsize);
      Log::Out("= 1");
    }
  else if(args[0] == "clear_board")
    {
      delete game;
      game = new GoGame(BOARD_DEFAULT_SIZE);
      Log::Out("= 1");
    }
  else if(args[0] == "play")
    {
      game->Play(ColorFromString(args[1]), ColumnStringToInt(args[2].substr(0,1)),
          RowStringToInt(args[2].substr(1,2)));
      Log::Out("= 1");
    }
  else if(args[0] == "genmove")
    {

      GoPoint pos = game->GenerateMove(ColorFromString(args[1]));

      std::stringstream ss;
      ss <<"= " << ColumnIntToString(pos.x)<<RowIntToString(pos.y);

      Log::Out(ss.str());

    }
  else if(args[0] == "showboard")
    {
      game->Board->DisplayCurrentState();
    }
#ifdef DEBUG_MODE
  else if(args[0] == "d")
    {
      if(args[1] == "libstone")
        {
          GoPoint p = GoPoint(ColumnStringToInt(args[2].substr(0,1)),RowStringToInt(args[2].substr(1,2)),B_WHITE);
          std::cout<<"= "<<game->Board->State.numNeighboursEmpty[game->Board->Pos(p)]<<"\n\n";
        }
      if(args[1] == "libblock")
        {
          GoPoint p = GoPoint(ColumnStringToInt(args[2].substr(0,1)),RowStringToInt(args[2].substr(1,2)),B_WHITE);
          std::cout<<"= "<<game->Board->State.blockPointers[game->Board->Pos(p)]->Liberties()<<"\n\n";
        }
    }
#endif //IFDEF DEBUG_MODE
  else
    throw Exception("Unknown command");
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
