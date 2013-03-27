
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
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


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
int fd;
char * fifo;

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

int initChildProc()
{
}


int main(int argc, char *argv[])
{
	LogLevel = ERROR;
	doTests = false;
	RAND_SEED = time(NULL);
	srand (RAND_SEED);

	int pipefd[10][2];
	char buf;


	pid_t pID;
	std::cerr<<"forkval "<<pID<<std::endl;
	for(int i = 0; i<10;++i)
	{
		if (pipe(pipefd[i]) == -1) {
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		if ((pID = fork()) == 0)                // child
		{
			std::cout<<"Child proc init";
			close(pipefd[i][1]); //Unused write
			while (read(pipefd[i][0], &buf, 1) > 0)
				write(STDOUT_FILENO, &buf, 1);
			write(STDOUT_FILENO, "\n", 1);
			close(pipefd[i][0]);
			_exit(EXIT_SUCCESS);
			initChildProc();
		}
		else if(pID < 0) //Fork failed
		{
			std::cout<<"FFail";
			exit(EXIT_FAILURE);
		}
	}

	//	std::cout<<"Parent proc init";
	for(int i = 0; i<10;++i)
	{
		close(pipefd[i][0]);          /* Close unused read end */
		write(pipefd[i][1], &itoa(i), strlen("1"));
		close(pipefd[i][1]);          /* Reader will see EOF */
	}
	wait();                /* Wait for child */
	exit(EXIT_SUCCESS);
	return initMainProc(argc,argv);

	return EXIT_SUCCESS;
}

