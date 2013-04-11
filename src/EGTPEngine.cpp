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

EGTPEngine::EGTPEngine():aiType(MC),numRandMoves(0),simulations(1000),timeAlloc(-1)
{

}

EGTPEngine::~EGTPEngine()
{
	// TODO Auto-generated destructor stub
}
extern int LogLevel;
std::string EGTPEngine::parse(std::string input)
{
	//Only returns in ifs on failure or on special responses. standard ack response returned if nothing else.
	std::cout<<"Attempting to parse EGTP input "<<input<<std::endl;
	std::vector<std::string> args;
	boost::split(args, input,boost::is_any_of( " " ));

	if(args[0] == "e_request")
	{
		if(args[1] != "1")
			_exit(EXIT_FAILURE);
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
		std::cout<<"Got genmove"<<std::endl;
		GoPoint p;
		std::vector<UCBrow> ucbr;
		switch(aiType)
		{
		case(UCB):
									{

			if(preselRandMoves.size()>0)
			{
				UpperConfidence ucb(preselRandMoves,simulations);
				ucbr = ucb.generateUCBTable(ColorFromString(args[1]),game);
				//				p = ucb.generateMove(ColorFromString(args[1]),game);
			}
			else
			{
				UpperConfidence ucb(numRandMoves,simulations);
				ucbr = ucb.generateUCBTable(ColorFromString(args[1]),game);
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
		ss << "= ucbtable:";
		for(int i = 0; i<ucbr.size();++i)
		{
			ss<<ucbr[i].pos<<","<<ucbr[i].expected<<","<<ucbr[i].timesPlayed<<";";
		}
		game->Board->reset();
		preselRandMoves.clear();
		return ss.str();
	}
	else
	{
		int oldLog = LogLevel;
		LogLevel = SILENT;
		std::vector<std::string> ret = GTPEngine::parse(input);
		LogLevel = oldLog;
	}
	return GTP_ACK_RESPONSE;
}

/*
GoPoint EGTPEngine::genMoveFromChildProcs(int color, GoGame* game)
{
	time_t timer;
	time(&timer);
	for(int i = 0; i<childProcs; ++i)
	{

		//Replay current state
		for(int j = 0; j<game->Board->movePointer; ++j)
		{
			char col = game->Board->moves[j]->Color == S_BLACK ? 'b' : 'w';
			std::stringstream ss;
			ss<<"play "<<col<<" "<<game->Board->ReadablePosition(game->Board->moves[j]->Point)<<"\n";
//			std::cerr<<ss.str()<<std::endl;
			PipeCommunication::writePipe(pipe_child[i][1],ss.str());
		}

		UpperConfidence ucb;
		std::vector<int> preselMoves = ucb.getPossibleMoves(color, game);

		std::string wbuf = GTPEngine::generateGTPString(game->Board);
		PipeCommunication::writePipe(pipe_child[i][1],wbuf);
		//		std::cout<<"Wrote "<<wbuf;
		std::stringstream ss;
		ss<<"e_randmoves";
		for(int j = 0;j<preselMoves.size();++j)
			ss<<" "<<preselMoves[j];
		ss<<"\n";
		LOG_VERBOSE<<"Wrote "<<ss.str()<<std::endl;
		PipeCommunication::writePipe(pipe_child[i][1],ss.str());
		wbuf = "e_useai ucb 0 s 1000\ngenmove b\n";
		PipeCommunication::writePipe(pipe_child[i][1],wbuf);
		LOG_VERBOSE<<"Wrote "<<wbuf;
	}

	std::vector<UCBrow> ucbtable;
	for(int i = 0; i<childProcs; ++i)
	{
		std::string str = PipeCommunication::readPipe(pipe_parent[i][0]);
		LOG_DEBUG<<"From child: "<<i<<" "<<str<<std::endl;
		std::vector<std::string> args;
		boost::split(args, str,boost::is_any_of( " " ));
		std::string ucbrstr = args[1];
		args.clear();
		boost::split(args, ucbrstr,boost::is_any_of( ";" ));
		for(int j = 0; j<args.size(); ++j)
		{
			std::vector<std::string> row;
			boost::split(row, args[j],boost::is_any_of( "," ));
			if(row[0].size()>0)
			{
				UCBrow u;
				u.pos = atoi(row[0].c_str());
				u.expected = atof(row[1].c_str());
				u.timesPlayed = atoi(row[2].c_str());
				bool exists = false;
				for(std::vector<UCBrow>::iterator it = ucbtable.begin(); it != ucbtable.end(); ++it)
				{

					if(atoi(row[0].c_str()) == (*it).pos)
					{
						//Recalculate current ucbrow;
						(*it).expected = ((u.expected*u.timesPlayed)+((*it).expected+(*it).timesPlayed))/(u.timesPlayed*(*it).timesPlayed);
						(*it).timesPlayed += u.timesPlayed;
						exists = true;
					}
				}
				if(!exists)
					ucbtable.push_back(u);
			}
		}
	}

	int bestPos = -1;
	float bestExpected = 0;
	int totalSims = 0;
	for(int i = 0; i<ucbtable.size(); ++i)
	{
		if(ucbtable[i].expected > bestExpected)
		{
			bestExpected = ucbtable[i].expected;
			bestPos = ucbtable[i].pos;
		}

		totalSims+=ucbtable[i].timesPlayed;
	}
	time_t now;
	time(&now);
	int perf = (float)totalSims/difftime(now,timer);
	std::cerr<<perf<<" sims per sec"<<std::endl;
	LOG_VERBOSE<<"Best move: "<<bestPos<<" based on "<<totalSims<<" simulations"<<std::endl;
	return game->Board->ReversePos(bestPos,color);

}

*/
