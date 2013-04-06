/*
 * MasterServer.h
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#ifndef MASTERSERVER_H_
#define MASTERSERVER_H_

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "TCPServer.h"

class MasterServer {
public:
	MasterServer(int port);
	void run();
	virtual ~MasterServer();
	void newConnection(boost::asio::ip::tcp::socket& socket);
	boost::asio::io_service io_service;
	TCPServer tcp_server;
private:


	std::vector<boost::asio::ip::tcp::socket*> sockets;
};

#endif /* MASTERSERVER_H_ */
