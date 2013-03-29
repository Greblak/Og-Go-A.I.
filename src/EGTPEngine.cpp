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
#include "PipeCommunication.h"

EGTPEngine::EGTPEngine(int outputPipe):aiType(MC),numRandMoves(0),simulations(1000),timeAlloc(-1),writePipe(outputPipe)
{

}

EGTPEngine::~EGTPEngine()
{
	// TODO Auto-generated destructor stub
}

std::vector<std::string> EGTPEngine::parse(std::string input)
{
	LOG_VERBOSE<<"Attempting to parse "<<input<<std::endl;
	std::vector<std::string> args;
	boost::split(args, input,boost::is_any_of( " " ));

	if(args[0] == "e_request")
	{
		if(args[1] == "1")
			LOG_OUT<<"= 1";

	}
	else if(args[0] == "e_randmoves")
	{
		for(int i = 1; i<args.size(); ++i)
		{
			preselRandMoves.push_back(atoi(args[i].c_str()));
		}
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
		numRandMoves= atoi(args[2].c_str());
	}
	else if(args[0]=="genmove")
	{
		GoPoint p;
		switch(aiType)
		{
		case(UCB):
		{

			if(preselRandMoves.size()>0)
			{
				UpperConfidence ucb(preselRandMoves,simulations);
				p = ucb.generateMove(ColorFromString(args[1]),game);
			}
			else
			{
				UpperConfidence ucb(numRandMoves,simulations);
				p = ucb.generateMove(ColorFromString(args[1]),game);
			}

			break;
		}
		case(MC):
				{
			MonteCarlo mc(numRandMoves,simulations);
			p = mc.generateMove(ColorFromString(args[1]),game);
			break;
				}
		}
		std::stringstream ss;
		ss << "= "<<game->Board->Pos(p)<<"\n";
		int w = PipeCommunication::writePipe(writePipe,ss.str());
		std::cout<<"wrote "<<ss.str()<<" "<<w<<std::endl;
	}
	else
		return GTPEngine::parse(input);
	return args;
}
