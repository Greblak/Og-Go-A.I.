
/***

Og-go A.I.
A kyu-level move analyzing A.I. for the boardgame Go.
By: Rune Borge Kalleberg
Project: DAT300 - Bachelor in computer science at the University of Agder

Version 0.1 - Basic framework, GTP communication and Random play algorithm

Planned milestones

Version 0.2 - Benson's algorithm for determining unconditional life
Version 0.3 - Begin work on move desiding algorithms. Type yet undefined.


***/
#include <iostream>
#include "GTPEngine.h"
#include "GoBoard.h"
#include "Exception.h"



int main(int argc, char *argv[])
{
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