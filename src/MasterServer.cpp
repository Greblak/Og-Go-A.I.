/*
 * MasterServer.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include <ctime>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "MasterServer.h"
#include "TCPServer.h"
#include "Log.h"
#include "EGTPEngine.h"
#include "GTPEngine.h"
#include "Exception.h"
#include "UpperConfidence.h"

extern std::string AI_CONFIG;
extern int AI_TYPE;

MasterServer::MasterServer(int port):tcp_server(io_service,port,boost::bind(&MasterServer::newConnection,this, _1)),writingToUcbTable(false),genmoveResponses(0),genmoveResponseWait(false),genmoveTimeoutMilliSec(10*1000),bestMove(-1,-1,NONE),commandNumber(1)
{
  
}
MasterServer::~MasterServer() {
  io_service.stop();
}

void MasterServer::ioServiceStarter()
{

  try
    {
      io_service.run();
    }
  catch (std::exception& e)
    {
      std::cout <<"Exception from io service: "<< e.what() << std::endl;
    }

}

void MasterServer::run()
{

  boost::thread workerThread(boost::bind(&MasterServer::ioServiceStarter,this));

  while(true)
    {
      try
	{
	  std::string userInput;
	  if(!std::cin.good())
	    std::cout<<"Bad!"<<std::endl;
	  std::getline(std::cin, userInput);
	  std::vector<std::string> args = gtp.parse(userInput);
	  if(args[0] == "genmove")
	    {
	      GoPoint p = generateMove(gtp.ColorFromString(args[1]));
	      if(p.IsPass())
		LOG_OUT<<"= PASS";
	      else
		{
		  LOG_OUT<< "= "<<gtp.game->Board->ReadablePosition(gtp.game->Board->Pos(p));
		  gtp.game->Play(p.color, p.x, p.y);
		}
	    }
	}
      catch( const char * str )
	{
	  LOG_ERROR <<"? Fatal exception raised: "<<str<<"\n\n"<<"Exiting...";
	  exit(EXIT_FAILURE);
	}
      catch (Exception& e)
	{
	  LOG_ERROR <<"? Fatal exception raised: "<<e.getMessage()<<"\n\n"<<"Exiting...";
	  exit(EXIT_FAILURE);
	}
    }
}

void MasterServer::newConnection(boost::shared_ptr<TCPConnection> conn)
{
  LOG_VERBOSE<<"New connection from "<<conn->socket().remote_endpoint().address().to_string()<<std::endl;
  doHandshake(conn);
  if(conn->socket().is_open())
    {
      tcp_server.sockets.push_back(conn);
    }

}

void MasterServer::doHandshake(boost::shared_ptr<TCPConnection> conn)
{
  conn->socket().write_some(boost::asio::buffer(EGTP_HANDSHAKE));
  //	socket_.write_some(boost::asio::buffer(EGTP_HANDSHAKE));
  boost::array<char, MASTER_BUFFER_MAX_SIZE> buf;
  boost::system::error_code ec;
  conn->socket().read_some(boost::asio::buffer(buf),ec);
  if(std::string(buf.data(),strlen(buf.data())) != GTP_ACK_RESPONSE)
    {
      LOG_DEBUG<<"Invalid EGTP version or malformed response. Closing connection"<<std::endl;
      conn->socket().close();
    }
}

void MasterServer::writeAll(const std::string str)
{
  ++commandNumber;
  if(tcp_server.sockets.size() == 0)
    throw "Unable to find any slaves";  
  int i = 0;
  std::stringstream ss;
  ss<<commandNumber<<" "<<str;
  std::cout<<"Sending: "<<ss.str();
  for(SocketVector::iterator it = tcp_server.sockets.begin(); it != tcp_server.sockets.end(); ++it)
    {
      std::cout<<"Accessing socket "<<i<<std::endl;
      if((*it)->is_open())
	{
	  try
	    {
	      write((*it),ss.str());
	    }
	  catch(char* ex)
	    {
	      LOG_ERROR<<ex<<std::endl;
	    }
	}
      else
	{
	  std::cout<<"Cleaning up dead socket "<<i<<" : "<<(*it)<<std::endl;
	  if(tcp_server.sockets.size() == 1)
	    tcp_server.sockets.clear();
	  else
	    {
	      it = tcp_server.sockets.erase(it); //Remove dead socket
	      --it;
	    }
	}
      i++;
    }
  
}

void MasterServer::write(boost::shared_ptr<TCPConnection> conn, const std::string str)
{
  //	std::cout<<"Trying to write "<<str<<std::endl;
  conn->socket().async_write_some(boost::asio::buffer(str),boost::bind(&MasterServer::writeHandler,this));
  conn->startTimeout();
  std::cout<<"Successfully wrote "<<str<<std::endl;
}

const GoPoint MasterServer::generateMove(int color)
{
  genmoveResponses = 0;
  genmoveResponseWait = true;
  bestMove = GoPoint(-1,-1,NONE);
  
  std::vector<std::string> aiconf;
  boost::split(aiconf, AI_CONFIG, boost::is_any_of(" "));
  int numRandom = 0;
  for(int i = 0; i<aiconf.size(); ++i)
    {
      if(aiconf[i] == "r")
	{
	  numRandom = atoi(aiconf[i+1].c_str());
	  if(numRandom < 1)
	    throw("Invalid number of generated possible moves. Must be > 0");
	  break;
	}
    }

  UpperConfidence ucb(numRandom,0);

  //Prepare new ucb-table
  std::vector<int> randMoves = ucb.getPossibleMoves(color,gtp.game);
  ucbTable.clear();
  for(int i = 0; i<randMoves.size(); ++i)
    {
      UCBrow r;
      r.pos = randMoves[i];
      r.expected = 0;
      r.timesPlayed = 0;
      ucbTable.push_back(r);
    }

  //Replay current state to all slaves
  std::stringstream ss;
  //Prepare string for slave writing
  std::string wbuf = GTPEngine::generateGTPString(gtp.game->Board);
  ss<<wbuf;
  ss<<"e_randmoves";
  for(int j = 0;j<randMoves.size();++j)
    ss<<" "<<randMoves[j];
  ss<<"\n";
  char col = color == S_BLACK ? 'b' : 'w';
  int timeRemainingForMove = gtp.game->TimeHandler.GetTimeRemainingForMove();
  if(timeRemainingForMove = GO_TIME_NOT_ACTIVE)
    ss<<"e_useai "<<AI_CONFIG<<"\n";
  else
    {
      ss<<"e_useai "<<AI_TYPE<<" t "<<timeRemainingForMove<<"\n";
    }
  ss<<"genmove "<<col<<"\n";
  writeAll(ss.str());
  int moveNumber = gtp.game->Board->movePointer; //Implement to prevent race conditions
  for(SocketVector::iterator it = tcp_server.sockets.begin(); it != tcp_server.sockets.end(); ++it)
    {
      boost::array<char, MASTER_BUFFER_MAX_SIZE>* buf = new boost::array<char, MASTER_BUFFER_MAX_SIZE>();
      buf->fill('\0');
      //		socketreads.push_back(buf);
      (*it)->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,(*it),buf));
    }
  int timestep = 100; //0.1 sec
  int timeout = timeRemainingForMove * 1000;
  while(genmoveResponseWait)
    {
      usleep(1000*timestep);
      timeout -= timestep;
      if(timeout == 0)
	break;
    }

  int bestPos = -1;

  float bestExpected = 0;
  int totalSims = 0;
    
  if(AI_TYPE != RAN)
    {
      for(int i = 0; i<ucbTable.size(); ++i)
	{
	  if(ucbTable[i].expected > bestExpected)
	    {

	      bestExpected = ucbTable[i].expected;
	      bestPos = ucbTable[i].pos;
	    }

	  totalSims+=ucbTable[i].timesPlayed;
	}
      LOG_VERBOSE<<"Best move: "<<bestPos<<" based on "<<totalSims<<" simulations"<<std::endl;
      LOG_VERBOSE<<"Sockets connected: "<<tcp_server.sockets.size()<<std::endl;
      bestMove = gtp.game->Board->ReversePos(bestPos,color);
     
      //LOG_OUT <<"= " << gtp.game->Board->ReadablePosition(pos);
    }
  else if(AI_TYPE == RAN)
    {
      //Do nothing, handled by async call 
    }
  return bestMove;
}

void MasterServer::genmoveReadCallback(boost::shared_ptr<TCPConnection> conn, boost::array<char, 1024>* buf)
{
  conn->resetTimeout();
  std::string input; 
  std::vector<std::string> responses;
  input = std::string(buf->c_array(), strlen(buf->c_array()));
  boost::split(responses, input,boost::is_any_of("\n"));
  for(std::vector<std::string>::const_iterator it = responses.begin(); it != responses.end(); ++it)
    {
      std::string input = (*it);
      if(strlen(input.c_str()) != 0)
	{
	  if(boost::starts_with(input, "="))
	    {
	      input = input.substr(1); //omit =
	      std::vector<std::string> args;
	      boost::split(args, input, boost::is_any_of(" "));
	      if(commandNumber == atoi(args[0].c_str()))
		{
		  args.erase(args.begin());
		  input = boost::algorithm::join(args," ");
		  //Parse input and confirm UCB table
		  if(input.find("ucbtable:") != std::string::npos)
		    {
		      while(writingToUcbTable)
			{
			  LOG_VERBOSE<<"No UCB table consultants are available right now. Please hold while we dig one up"<<std::endl;
			}
		      writingToUcbTable = true;
		      std::vector<UCBrow> incomingUCBTable = UpperConfidence::parseUCBTableString(input);
		      UpperConfidence::combineUCBTables(ucbTable, incomingUCBTable);
		      writingToUcbTable = false;
		      ++genmoveResponses;
		      if(genmoveResponses>=tcp_server.sockets.size())
			genmoveResponseWait = false;
		    }
		  else if(input.find("rand:") != std::string::npos) //Random ai used
		    {
		      std::string pos = input.substr(7);
		      const int x = gtp.ColumnStringToInt(pos);
		      const int y = gtp.RowStringToInt(pos);
		      bestMove.x = x;
		      bestMove.y = y;
		      bestMove.color = NONE;
		      genmoveResponseWait = false;
		    }
		  else if(args.size() == 0)
		    {
		      LOG_VERBOSE<<"ACK from client"<<std::endl;
		    }
		  else
		    {
		      LOG_VERBOSE<<"Unable to parse response"<<std::endl;
		    }
		}
	      else
		{
		  LOG_VERBOSE<<"Response omitted. Outdated."<<std::endl;
		}
	    }
	  
	}
    }
  if(strlen(buf->c_array()) == 0)
    return;
  buf->fill('\0');
  conn->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,conn,buf));
    
}

void MasterServer::writeHandler()
{
  //	std::cout<<"Written async"<<std::endl;
}

void MasterServer::checkDeadConnections()
{
  //  for(SocketVector::iterator it = sockets.begin(); it != sockets.end; ++it)
  //    {
  //      (*it).write_some(boost::asio::buffer("e_ping\n"));
  //      boost::array<char, MASTER_BUFFER_MAX_SIZE> buf;
  //      sleep(5);
  //      (*it).read_some(boost::asio::buffer(buf));		       
  //      if(std::string(buf.data(),strlen(buf.data())) != GTP_ACK_RESPONSE)
  //	{
  //	  std::cout<<"Connection to socket timed out."<<std::endl;
  //	  it = sockets.erase(it);
  //	}
  //
  //    }
}

