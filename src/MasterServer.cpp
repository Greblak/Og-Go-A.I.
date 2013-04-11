/*
 * MasterServer.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include <ctime>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include "MasterServer.h"
#include "TCPServer.h"
#include "Log.h"
#include "EGTPEngine.h"
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "GTPEngine.h"
#include "Exception.h"
#include "UpperConfidence.h"
MasterServer::MasterServer(int port):tcp_server(io_service,port,boost::bind(&MasterServer::newConnection,this, _1)),writingToUcbTable(false)
{

	try
	{
		boost::thread workerThread(boost::bind(&boost::asio::io_service::run, &io_service));
	}
	catch (std::exception& e)
	{
		LOG_ERROR << e.what() << std::endl;
	}
}

MasterServer::~MasterServer() {
	io_service.stop();
}

void MasterServer::run()
{
	while(true)
	{
		try
		{
			std::string userInput;
			std::getline(std::cin, userInput);
			std::vector<std::string> args = gtp.parse(userInput);
			if(args[0] == "genmove")
			{
				GoPoint p = generateMove(gtp.ColorFromString(args[1]));
				LOG_OUT<< "= "<<gtp.game->Board->ReadablePosition(gtp.game->Board->Pos(p));
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
	std::cout<<"New connection from "<<conn->socket().remote_endpoint().address().to_string()<<std::endl;
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
		std::cout<<"Invalid EGTP version or malformed response. Closing connection"<<std::endl;
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
				std::cout<<ex<<std::endl;
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
	UpperConfidence ucb;
	std::vector<int> randMoves = UpperConfidence().getPossibleMoves(color,gtp.game);
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

	std::string wbuf = GTPEngine::generateGTPString(gtp.game->Board);
	writeAll(wbuf);
	std::stringstream ss;
	ss<<"e_randmoves";
	for(int j = 0;j<randMoves.size();++j)
		ss<<" "<<randMoves[j];
	ss<<"\n";
	std::cout<<"Sending "<<randMoves.size()<< "moves :"<<ss.str()<<std::endl;
	writeAll(ss.str());
	wbuf = "e_useai ucb 0 s 200\ngenmove b";
	writeAll(wbuf);

	int moveNumber = gtp.game->Board->movePointer; //Implement to prevent race conditions

	for(SocketVector::iterator it = sockets.begin(); it!= sockets.end(); ++it)
	{
		//Build errors
		boost::array<char, MASTER_BUFFER_MAX_SIZE>* buf = new boost::array<char, MASTER_BUFFER_MAX_SIZE>();
		buf->fill('\0');
		//		socketreads.push_back(buf);
		(*it)->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,(*it),buf));
	}

	sleep(10);

	int bestPos = -1;
	float bestExpected = 0;
	int totalSims = 0;
	std::cout<<"UCB table size: "<<ucbTable.size()<<std::endl;
	for(int i = 0; i<ucbTable.size(); ++i)
	{
		std::cout<<"P: "<<ucbTable[i].pos<<" E:"<<ucbTable[i].expected<<" N:"<<ucbTable[i].timesPlayed<<std::endl;
		if(ucbTable[i].expected > bestExpected)
		{

			bestExpected = ucbTable[i].expected;
			bestPos = ucbTable[i].pos;
		}

		totalSims+=ucbTable[i].timesPlayed;
	}
	std::cout<<"Best move: "<<bestPos<<" based on "<<totalSims<<" simulations"<<std::endl;
	return gtp.game->Board->ReversePos(bestPos,color);


	GoPoint pos;
	gtp.game->Play(pos.color, pos.x,pos.y);
	LOG_OUT <<"= " << gtp.game->Board->ReadablePosition(pos);

	return pos;
}

void MasterServer::genmoveReadCallback(boost::shared_ptr<TCPConnection> conn, boost::array<char, 1024>* buf)
{
	std::cout<<"async callback"<<std::endl;
	std::string ucbstring;
	if(buf->size() == 0 || buf->elems[0] == '\0')
	{
		std::cout<<"Empty buf"<<std::endl;
	}
	else
	{
		std::cout<<"Buffer has read data!"<<buf->c_array()<<std::endl;
		ucbstring = std::string(buf->c_array(), strlen(buf->c_array()));
	}
	if(!boost::starts_with(ucbstring, "= 1"))
	{
		std::cout<<"mutex loop"<<std::endl;
		while(writingToUcbTable)
		{
			std::cout<<"No UCB table consultants are available right now. Please hold while we dig one up"<<std::endl;
		}
		std::cout<<"EO mutex loop"<<std::endl;
		writingToUcbTable = true;
		std::cout<<"Presize"<<ucbTable[0].timesPlayed<<std::endl;
		std::vector<UCBrow> incomingUCBTable = UpperConfidence::parseUCBTableString(ucbstring);
		UpperConfidence::combineUCBTables(ucbTable, incomingUCBTable);
		std::cout<<"Postsize"<<ucbTable[0].timesPlayed<<std::endl;
		writingToUcbTable = false;
	}
	buf->fill('\0');
	std::cout<<"starting new async read"<<std::endl;
	conn->socket().async_read_some(boost::asio::buffer(*buf),boost::bind(&MasterServer::genmoveReadCallback,this,conn,buf));
	std::cout<<"finished reading"<<std::endl;
}

void MasterServer::writeHandler()
{
	//	std::cout<<"Written async"<<std::endl;
}


