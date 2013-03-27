/*
 * EGTPEngine.cpp
 *
 *  Created on: Mar 27, 2013
 *      Author: rune
 */

#include <boost/algorithm/string.hpp>
#include "EGTPEngine.h"
#include "Log.h"
#include "UpperConfidence.h"

EGTPEngine::EGTPEngine():aiType(MC),randMoves(20),simulations(1000),timeAlloc(-1)
{
	// TODO Auto-generated constructor stub

}

EGTPEngine::~EGTPEngine()
{
	// TODO Auto-generated destructor stub
}

std::vector<std::string> EGTPEngine::parse(std::string input)
{
	std::vector<std::string> args;
	boost::split(args, input,boost::is_any_of( " " ));

	if(args[0] == "e_request")
	{
		if(args[1] == "1")
			LOG_OUT<<"= 1";

	}
	else if(args[0] == "e_useai") //Usage: e_useai [ran|mc|ucb|uct] [ai specific args]
	{
		if(args[1] == "ucb") //args: e_useai ucb [random moves] [t|s] [time in sec|num simulations] //t uses time, s uses simulations
		{
			aiType = UCB;
			if(args[3] == "t") //Time alloc
				timeAlloc = atoi(args[4].c_str());
			if(args[3] == "s") //simulations
				simulations = atoi(args[4].c_str());
		}

		randMoves = atoi(args[2].c_str());
	}
	else if(args[0]=="genmove")
	{
		switch(aiType)
		{
		case(UCB):
			{
			UpperConfidence ucb(randMoves,simulations);
			ucb.generateMove(ColorFromString(args[1]),game);
			break;
			}
		case(MC):
		{
			break;
		}
		}
		GoPoint pos = game->GenerateMove(ColorFromString(args[1]));
	}
	else
		return GTPEngine::parse(input);
	return args;
}
