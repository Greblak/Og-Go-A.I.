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

GTPEngine::GTPEngine(void)
{
	game = NULL;
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
		Log::Out("= name\nversion\nprotocol_version\nlist_commands\nboardsize\ngenmove\nplay\nclear_board");
	}
	else if(args[0] == "boardsize")
	{
		if(game != NULL)
			throw "Boardsize has already been set.";

		int bsize;
		try
			{
				bsize = atoi(args[1].c_str());
			}
		catch( char * str)
		{
			throw "Invalid boardsize. NaN";
		}

		game = new GoGame(bsize);	
		Log::Out("= 1");
	}
	else if(args[0] == "clear_board")
	{
		Log::Out("= 1");
	}
	else if(args[0] == "play")
	{
		game->Play(ColorFromString(args[1]), ColumnStringToInt(args[2].substr(0,1)), 
			RowStringToInt(args[2].substr(1,1)));
		Log::Out("= 1");
	}
	else if(args[0] == "genmove")
	{
		GoPoint pos = game->GenerateMove(ColorFromString(args[1]));
	}
	else
		throw "Unknown command";
}

const int GTPEngine::ColumnStringToInt(std::string str) const
{
	int n = str[0];
	
	if(n >= GTP_OFFSET_UPPERCASE_A)
		n = n - GTP_OFFSET_UPPERCASE_A;
	else
		n = n - GTP_OFFSET_LOWERCASE_A;

	if(n < 0 || n > BOARD_MAX_SIZE || n == GTP_OFFSET_I)
		throw "Invalid board column. ";

	if(n>= GTP_OFFSET_I)
		n--;

	return n;
}

const int GTPEngine::RowStringToInt(std::string str) const
{
	int n = str[0] - GTP_OFFSET_NUM;

	if(n < 0 || n > BOARD_MAX_SIZE)
		throw "Invalid board row. ";

	return n;
}

const int GTPEngine::ColorFromString(std::string str) const
{
	if(str == "w" || str == "W" || str == "white" || str == "WHITE")
		return B_BLACK;
	else if(str == "b" || str == "B" || str == "black" || str == "BLACK")
		return B_WHITE;
	else
		throw "Error converting string to BoardColor";
}