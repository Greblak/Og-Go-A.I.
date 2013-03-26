
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
	if (mkfifo(fifo, 0666) < 0)
	        perror("mkfifo");
	std::cout<<"Child proc init";
	fd = open(fifo, O_WRONLY);
	std::cout<<"fd:"<<fd;
	char* dat = "Hi";
	int w = write(fd, dat, sizeof(dat));
	std::cout<<"wrote bytes:"<<w<<dat;
	close(fd);

}


int main(int argc, char *argv[])
{
	LogLevel = ERROR;
	doTests = false;
	RAND_SEED = time(NULL);
	srand (RAND_SEED);
	fifo = "/tmp/myfifo";

	pid_t pID = fork();
	std::cerr<<"forkval "<<pID<<std::endl;
	if (pID == 0)                // child
	{
		std::cout<<"1";
		initChildProc();
	}
	else if(pID < 0) //Fork failed
	{
		std::cout<<"FFail";
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout<<"Parent proc init";
		char dat[4];
		wait();
		std::cout<<"Child done";
		fd = open(fifo, O_RDONLY);
		std::cout<<"Fdopen"<<fd<<" ";
		int r = read(fd, &dat, sizeof("Hi"));
		std::cout<<"Read:"<<r;
		std::cout<<"read";
		close(fd);
		std::cout<<"closed";
		std::cout<<dat[0];
		unlink(fifo);
		std::cout<<"Unlinked "<<fifo;
		return initMainProc(argc,argv);
	}
	return EXIT_SUCCESS;
}

