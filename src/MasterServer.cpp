/*
 * MasterServer.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include "MasterServer.h"
#include "TCPServer.h"
#include "Log.h"
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "GTPEngine.h"
#include "Exception.h"
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
	GTPEngine gtp;
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
				exit(EXIT_FAILURE);
			}
			catch (Exception& e)
			{
				LOG_ERROR <<"? Fatal exception raised: "<<e.getMessage()<<"\n\n"<<"Exiting...";
				exit(EXIT_FAILURE);
			}
		}
}

void MasterServer::newConnection(boost::asio::ip::tcp::socket& socket_)
{
	std::cout<<"New connection from "<<socket_.remote_endpoint().address().to_string()<<std::endl;
	sockets.push_back(&socket_);
}
