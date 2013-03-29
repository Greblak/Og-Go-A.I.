
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
#include "EGTPEngine.h"
#include "GoBoard.h"
#include "Exception.h"
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "GoBoard.h"
#include "UpperConfidence.h"
#include "PipeCommunication.h"
#include <boost/algorithm/string.hpp>
#include "Tests.h"

int LogLevel;
bool doTests;
#include "Log.h"

/*
Arguments taken:
- Debugging levels
	-v verbose
	-d debug
 */

int RAND_SEED = 1;
bool keepRand = false;


//child writes to child and reads from parent
//parent writes to parent and reads from child
int pipe_child[4][2];
int pipe_parent[4][2];

//Used to initialise mother process

int initMainProc(int argc, char *argv[])
{

	std::cout<<"Init main proc";
	for (int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-v") == 0)
			LogLevel = VERBOSE;
		if(strcmp(argv[i], "-d") == 0)
			LogLevel = DEBUG;
		if(strcmp(argv[i], "-t") == 0)
			doTests = true;
		if(strcmp(argv[i], "-r") == 0)
		{
			RAND_SEED = atoi(argv[i+1]);
			keepRand = true;
		}
	}
	LOG_VERBOSE << "Built " << __DATE__ << " - " << __TIME__;

	if(doTests)
	{
		//      TEST_PlayPolicy();
		//      TEST_UpperConfidence();
		TEST_EGTPEngine();
		std::string line = "test";
		std::ifstream file;
		file.open("test.gtp");
		if(file.fail())
		{
			LOG_DEBUG << "Test file failed to open. Business as usual";
		}
		int i = 0;
		return(EXIT_SUCCESS);
	}

	GTPEngine gtp = GTPEngine();
	while(true)
	{
		try
		{
			std::string userInput;
			std::getline(std::cin, userInput);
			gtp.parse(userInput);
		}
		catch( const char * str )
		{
			LOG_ERROR <<"? Fatal exception raised: "<<str<<"\n\n"<<"Exiting...";
			return 1;
		}
		catch (Exception e)
		{
			LOG_ERROR <<"? Fatal exception raised: "<<e.getMessage()<<"\n\n"<<"Exiting...";
			return 1;
		}
	}
	//	unlink(fifo);
}

int initChildProc(int procID)
{
	close(pipe_parent[procID][0]);
	//	close(pipe_child[procID][0]);
	close(pipe_child[procID][1]);

	EGTPEngine gtp(pipe_parent[procID][1]);
	std::string input = "";
	std::vector<std::string> cmd;
	while(true)
	{
		input = PipeCommunication::readPipe(pipe_child[procID][0]);
		cmd = gtp.parse(input);
		input = "";

		if(cmd[0] == "genmove" || cmd[0] == "genmove")
			break;
	}

	std::cout<<"Child proc "<<procID<<" signing out"<<std::endl;
	exit(EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
	LogLevel = SILENT;
	doTests = false;
	RAND_SEED = time(NULL);
	srand (RAND_SEED);

		pid_t pID;
		std::cerr<<"forkval "<<pID<<std::endl;
		for(int i = 0; i<1;++i)
		{
			if (pipe(pipe_child[i]) == -1 || pipe(pipe_parent[i]) == -1){
				perror("pipe");
				exit(EXIT_FAILURE);
			}
			if ((pID = fork()) == 0)                // child
			{
				initChildProc(i);
			}
			else if(pID < 0) //Fork failed
			{
				std::cout<<"FFail";
				exit(EXIT_FAILURE);
			}
		}

		//	std::cout<<"Parent proc init";
		for(int i = 0; i<1;++i)
		{
			close(pipe_child[i][0]);
			//		close(pipe_child[i][1]);
			GoGame g(9);
			g.Play(S_BLACK,3,3);
			g.Play(S_WHITE,2,3);
			std::vector<int> randMoves;
			randMoves.push_back(1);
			randMoves.push_back(3);
			randMoves.push_back(4);
			randMoves.push_back(5);
			std::string wbuf = GTPEngine::generateGTPString(g.Board);
			PipeCommunication::writePipe(pipe_child[i][1],wbuf);
			//		std::cout<<"Wrote "<<wbuf;
			std::stringstream ss;
			ss<<"e_randmoves";
			for(int j = 0;j<randMoves.size();++j)
				ss<<" "<<randMoves[j];
			ss<<"\n";
			PipeCommunication::writePipe(pipe_child[i][1],ss.str());
			wbuf = "e_useai ucb 0 s 100\ngenmove b\n";
			PipeCommunication::writePipe(pipe_child[i][1],wbuf);
			//		std::cout<<"Wrote "<<wbuf;
			close(pipe_child[i][1]);
		}
		for(int i = 0; i<1;++i)
		{
			char ic = i+65;
			std::string str = PipeCommunication::readPipe(pipe_parent[i][0]);
			std::cout<<"From child: "<<i<<" "<<str<<std::endl;
		}
	//	exit(EXIT_SUCCESS);
	return initMainProc(argc,argv);

	return EXIT_SUCCESS;
}

