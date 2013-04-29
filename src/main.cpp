
/***

    Og-go A.I.
    A kyu-level move analyzing A.I. framework for the boardgame Go.
    By: Rune Borge Kalleberg
    Project: DAT300 - Bachelor in computer science at the University of Agder

***/

#include <iostream>
#include <cstring>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/system/error_code.hpp>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "Config.h"
#include "EGTPEngine.h"
#include "GoBoard.h"
#include "Exception.h"
#include "GoBoard.h"
#include "UpperConfidence.h"
#include "PipeCommunication.h"
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
  -t //perform tests
  -r <seed> //sets random seed
  -n <hostname/ip> //set program as network node
  -ai {<ai type> <ai specific args> } //sets ai mode for this session (UCB with 200 simulations by default) - examples: -ai { ucb s 1000 r 20 } will run 1000 sims for each child in ucb. 20 moves will be selected randomly in addition to any local moves, t 10 will run as many simulations as possible in 10 seconds before returning to master. The same applies to mc-ai
  
*/

int RAND_SEED = 1;
bool keepRand = false;
int LogLevel = ERROR;
std::string AI_CONFIG = "ucb t 10 r 20";
int AI_TYPE = UCB;

extern std::string AI_CONFIG;
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
      if(strcmp(argv[i], "-ai") == 0) //Set ai type and arguments
	{
	  if(strcmp(argv[i+1], "{") == 0)
	    {
	      std::stringstream ss;
	      int j = i+2;
	      if(strcmp(argv[i+2], "ucb") == 0)
		AI_TYPE = UCB;
	      else if(strcmp(argv[i+2], "mc") == 0)
		AI_TYPE = MC;
	      else if(strcmp(argv[i+2], "rand") == 0)
		AI_TYPE = RAN;
	      do
		{
		  ss<<argv[j]<<" ";
		  ++j;
		}
	      while(strcmp(argv[j], "}") != 0);
	      AI_CONFIG = ss.str();
	    }
	}
    }
  if(isSlave)
    {
      return initSlave(hostip,port);
    }
  else if(doTests)
    {
      TEST_DOTESTS();
      /*      std::string line = "test";
      std::ifstream file;
      file.open("test.gtp");
      if(file.fail())
	{
	  LOG_DEBUG << "Test file failed to open. Business as usual";
	}
      int i = 0;
      */
      return(EXIT_SUCCESS);
    }
  return initMaster(port);
}
