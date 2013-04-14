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

MasterServer::MasterServer(int port):tcp_server(io_service,port,boost::bind(&MasterServer::newConnection,this, _1)),writingToUcbTable(false),genmoveResponses(0),genmoveResponseWait(false),genmoveTimeoutMilliSec(10*1000),bestMove(-1,-1,NONE)
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
	      LOG_OUT<< "= "<<gtp.game->Board->ReadablePosition(gtp.game->Board->Pos(p));
	      gtp.game->Play(p.color, p.x, p.y);
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
      sockets.push_back(conn);
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
  for(SocketVector::iterator it = sockets.begin(); it != sockets.end(); ++it)
    {
      if((**it).socket().is_open())
	{
	  //			std::cout<<"Trying to write to "<<&(**it);
	  try
	    {
	      write((*it),str);
	    }
	  catch(char* ex)
	    {
	      LOG_ERROR<<ex<<std::endl;
	    }
	}
      else
	{
	  it = sockets.erase(it); //Remove dead socket
	}
    }
}

void MasterServer::write(boost::shared_ptr<TCPConnection> conn, const std::string str)
{
  //	std::cout<<"Trying to write "<<str<<std::endl;
  conn->socket().async_write_some(boost::asio::buffer(str),boost::bind(&MasterServer::writeHandler,this));
  //	std::cout<<"Successfully wrote "<<str<<std::endl;
}

const GoPoint MasterServer::generateMove(int color)
{
  genmoveResponses = 0;
  genmoveResponseWait = true;
  bestMove = GoPoint(-1,-1,NONE);

  UpperConfidence ucb;

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
  writeAll(GTP_CMD_CLEAR_BOARD);
  for(int j = 0; j<gtp.game->Board->movePointer; ++j)
    {
      char col = gtp.game->Board->moves[j]->Color == S_BLACK ? 'b' : 'w';
      std::stringstream ss;
      ss<<"play "<<col<<" "<<gtp.game->Board->ReadablePosition(gtp.game->Board->moves[j]->Point)<<"\n";

      writeAll(ss.str());
    }

  //Prepare string for slave writing
  std::string wbuf = GTPEngine::generateGTPString(gtp.game->Board);
  writeAll(wbuf);
  std::stringstream ss;
  ss<<"e_randmoves";
  for(int j = 0;j<randMoves.size();++j)
    ss<<" "<<randMoves[j];
  ss<<"\n";
  char col = color == S_BLACK ? 'b' : 'w';
  ss<<"e_useai "<<AI_CONFIG<<"\ngenmove "<<col<<"\n";
  writeAll(ss.str());

  std::cout <<ss.str()<<std::endl;


  int moveNumber = gtp.game->Board->movePointer; //Implement to prevent race conditions

  for(SocketVector::iterator it = sockets.begin(); it!= sockets.end(); ++it)
    {
      //Build errors
      boost::array<char, MASTER_BUFFER_MAX_SIZE>* buf = new boost::array<char, MASTER_BUFFER_MAX_SIZE>();
      buf->fill('\0');
      //		socketreads.push_back(buf);
      (*it)->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,(*it),buf));
    }
  int timestep = 100; //0.1 sec
  int timeout = genmoveTimeoutMilliSec;
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
      std::cout<<"Sockets connected: "<<sockets.size()<<std::endl;
      bestMove = gtp.game->Board->ReversePos(bestPos,color);
     
      //LOG_OUT <<"= " << gtp.game->Board->ReadablePosition(pos);
    }
  else if(AI_TYPE == RAN)
    {
      //Do nothing, handled by async call 
    }
  gtp.game->Play(bestMove.color, bestMove.x,bestMove.y);
  return bestMove;
}

void MasterServer::genmoveReadCallback(boost::shared_ptr<TCPConnection> conn, boost::array<char, 1024>* buf)
{

  std::string input; 
  std::vector<std::string> responses;
  input = std::string(buf->c_array(), strlen(buf->c_array()));
  boost::split(responses, input,boost::is_any_of("\n"));
  for(std::vector<std::string>::const_iterator it = responses.begin(); it != responses.end(); ++it)
    {
      std::string input = (*it);
      if(strlen(input.c_str()) != 0)
	{
	  if(!boost::starts_with(input, "= 1"))
	    {
	      //Parse input and confirm UCB table
	      if(input.find("= ucbtable:") != std::string::npos)
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
		  std::cout<<genmoveResponses<< " "<<sockets.size()<<std::endl;
		  if(genmoveResponses>=sockets.size())
		    genmoveResponseWait = false;
		}
	      else if(input.find("= rand:") != std::string::npos) //Random ai used
		{
		  std::string pos = input.substr(7);
		  const int x = gtp.ColumnStringToInt(pos);
		  const int y = gtp.RowStringToInt(pos);
		  bestMove.x = x;
		  bestMove.y = y;
		  bestMove.color = NONE;
		  genmoveResponseWait = false;
		}
	      else
		{
		  std::cout<<"unable to parse input: "<<input<<std::endl;
		}
	    }
	  buf->fill('\0');
	  conn->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,conn,buf));
	}
    }
    
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

