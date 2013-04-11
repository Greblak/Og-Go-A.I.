
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
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/system/error_code.hpp>
#include "Tests.h"
#include "Log.h"
#include "TCPServer.h"
#include "MasterServer.h"
#include "SlaveClient.h"

/*
Arguments taken:
- Debugging levels
	-v verbose
	-d debug
-t perform tests
-r $1 random seed
-n network node

 */

int RAND_SEED = 1;
bool keepRand = false;
int LogLevel = ERROR;


int initMaster(int port)
{
	MasterServer master(port);
	master.run();
	return EXIT_SUCCESS;
}

int initSlave(std::string serverip, int port)
{
	LOG_VERBOSE << "Attempting to start slave client";
	SlaveClient slave(serverip, port);
	slave.run();
	return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
	RAND_SEED = time(NULL);
	srand (RAND_SEED);
	bool isSlave = false;
	bool doTests = false;
	std::string hostip = NETWORK_DEFAULT_MASTER_IP;
	int port = NETWORK_DEFAULT_MASTER_PORT;

	for (int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-v") == 0) //Set verbose mode
			LogLevel = VERBOSE;
		if(strcmp(argv[i], "-d") == 0) //Set debug mode
			LogLevel = DEBUG;
		if(strcmp(argv[i], "-t") == 0) //Perform tests prior to start
			doTests = true;
		if(strcmp(argv[i], "-r") == 0) //Random seed $1
		{
			RAND_SEED = atoi(argv[i+1]);
			keepRand = true;
		}
		if(strcmp(argv[i], "-n") == 0) //network node
		{
			isSlave = true;
			boost::system::error_code ec;
			boost::asio::ip::address::from_string( argv[i+1], ec );
			if ( !ec )
				hostip = argv[i+1];
		}
		if(strcmp(argv[i], "-p") == 0) //network node
		{
			port = atoi(argv[i+1]);
		}
	}
	if(isSlave)
	{
		return initSlave(hostip,port);
	}
	else if(doTests)
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
	return initMaster(port);
}

}
