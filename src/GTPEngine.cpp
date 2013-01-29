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

GTPEngine::GTPEngine(void)
{
	game = new GoGame(BOARD_DEFAULT_SIZE);
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
		
		std::stringstream ss;
		ss <<"= " << ColumnIntToString(pos.x)<<RowIntToString(pos.y);
		
		Log::Out(ss.str());

	}
	else if(args[0] == "showboard")
	{
		game->Board->DisplayCurrentState();
	}
	else
		throw Exception("Unknown command");
}

const int GTPEngine::ColumnStringToInt(std::string str) const
{
	int n = str[0];
	
	if(n >= GTP_OFFSET_UPPERCASE_A)
		n = n - GTP_OFFSET_UPPERCASE_A;
	else
		n = n - GTP_OFFSET_LOWERCASE_A;

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
	int n = (str[1] - GTP_OFFSET_NUM);
	if(n>9)
		throw Exception("Invalid row");

	std::cout<<str<<" "<<n<< " " ;
	if(str[2] != 0)
	{
		n*=10;
		n+= str[2] - GTP_OFFSET_NUM;
	}
	n--;

	std::cout<< " to "<<n<<"\n";
	if(n < 0 || n >= BOARD_MAX_SIZE)
		throw Exception("Invalid row.");

	return n;
}

const int GTPEngine::ColorFromString(std::string str) const
{
	if(str == "w" || str == "W" || str == "white" || str == "WHITE")
		return B_BLACK;
	else if(str == "b" || str == "B" || str == "black" || str == "BLACK")
		return B_WHITE;
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