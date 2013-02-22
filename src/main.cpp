
/***

Og-go A.I.
A kyu-level move analyzing A.I. for the boardgame Go.
By: Rune Borge Kalleberg
Project: DAT300 - Bachelor in computer science at the University of Agder

Version 0.1 - Basic framework, GTP communication and Random play algorithm

Planned milestones

Version 0.2 - Captures stones and connects blocks
Version 0.3 - Benson's algorithm for determining unconditional life
Version 0.4 - Begin work on move-deciding algorithms. Type yet undefined.


***/

#include <iostream>
#include <cstring>
#include "Config.h"
#include "GTPEngine.h"
#include "GoBoard.h"
#include "Exception.h"


int LogLevel;
#include "Log.h"

/*
Arguments taken:
- Debugging levels
	-v verbose
	-d debug
*/


int main(int argc, char *argv[])
{
  LogLevel = ERROR;
	for (int i = 1; i < argc; i++)
	{
	 if(strcmp(argv[i], "-v") == 0)
	 	LogLevel = VERBOSE;
	 if(strcmp(argv[i], "-d") == 0)
	 	LogLevel = DEBUG;
	}

	Log::Out("Starting GTP Engine");
	GTPEngine gtp = GTPEngine();
	while(true)
	{
		try
		{
			gtp.parse();	
		}
		catch( const char * str )
		{
			std::cerr<<"? Fatal exception raised: "<<str<<"\n\n"<<"Exiting...";
			return 1;
		}
		catch (Exception e)
		{
			std::cerr<<"? Fatal exception raised: "<<e.getMessage()<<"\n\n"<<"Exiting...";
			return 1;
		}
	}
	
}
