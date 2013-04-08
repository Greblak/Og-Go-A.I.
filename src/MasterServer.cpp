/*
 * MasterServer.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include <ctime>
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
MasterServer::MasterServer(int port):tcp_server(io_service,port,boost::bind(&MasterServer::newConnection,this, _1))
{
	try
	{
		io_service.run();
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

void MasterServer::newConnection(boost::asio::ip::tcp::socket* socket_)
{
	std::cout<<"New connection from "<<socket_->remote_endpoint().address().to_string()<<std::endl;
	doHandshake(*socket_);
	if(socket_->is_open())
		sockets.push_back(socket_);
}

void MasterServer::doHandshake(boost::asio::ip::tcp::socket& socket_)
{
	socket_.write_some(boost::asio::buffer(EGTP_HANDSHAKE));
	boost::array<char, MASTER_BUFFER_MAX_SIZE> buf;
	boost::system::error_code ec;
	socket_.read_some(boost::asio::buffer(buf),ec);
	if(std::string(buf.data(),strlen(buf.data())) != GTP_ACK_RESPONSE)
		socket_.close();
}

void MasterServer::writeAll(const std::string str)
{
	for(SocketVector::iterator it = sockets.begin(); it != sockets.end(); ++it)
	{
		if((*it)->is_open())
		{
			write(**it,str);
		}
		else
		{
			it = sockets.erase(it); //Remove dead socket
		}
	}
}

void MasterServer::write(boost::asio::ip::tcp::socket& socket, const std::string str)
{
	socket.write_some(boost::asio::buffer(str));
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
	writeAll(ss.str());
	wbuf = "e_useai ucb 0 s 1000\ngenmove b\n";
	writeAll(wbuf);


	std::vector<UCBrow> ucbtable;
	int moveNumber = gtp.game->Board->movePointer; //Implement to prevent race conditions

	for(SocketVector::iterator it = sockets.begin(); it!= sockets.end(); ++it)
	{
		boost::array<char, MASTER_BUFFER_MAX_SIZE>* buf = new boost::array<char, MASTER_BUFFER_MAX_SIZE>();
		socketreads.push_back(buf);
		(*it)->async_read_some(*buf,&MasterServer::genmoveReadCallback);
	}

	sleep(10);

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
	LOG_VERBOSE<<"Best move: "<<bestPos<<" based on "<<totalSims<<" simulations"<<std::endl;
	return gtp.game->Board->ReversePos(bestPos,color);


	GoPoint pos;
	gtp.game->Play(pos.color, pos.x,pos.y);
	LOG_OUT <<"= " << gtp.game->Board->ReadablePosition(pos);
	if(sockets.size()>0) //Has active nodes. Attempt to do locally
	{

	}
	else //Create locally
	{

	}
}

void MasterServer::genmoveReadCallback()
{
	std::cout<<"Got callback from read. Checking buffers"<<std::endl;
	for(SocketReadVector::iterator it = socketreads.begin(); it != socketreads.end(); ++it)
	{
		if((*it)->at(0) == 0)
		{
			std::cout<<"Empty buf"<<std::endl;
		}
		else
		{
			std::cout<<"Buffer has read data!"<<std::endl;
			std::string ucbstring((*it)->begin(), strlen((*it)->begin()));
			ucbTable = UpperConfidence::combineUCBTables(ucbTable, UpperConfidence::parseUCBTableString(ucbstring));
		}
	}
}


