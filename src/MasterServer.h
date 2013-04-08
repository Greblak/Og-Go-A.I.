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

typedef std::vector<boost::asio::ip::tcp::socket*> SocketVector;

const int MASTER_BUFFER_MAX_SIZE = 1024;

class MasterServer {
public:
	MasterServer(int port);
	void run();
	virtual ~MasterServer();
	void newConnection(const boost::asio::ip::tcp::socket& socket);
	void doHandshake(const boost::asio::ip::tcp::socket& socket);
	void write(const boost::asio::ip::tcp::socket& socket, const std::string str);
	void writeAll(const std::string str);
	const GoPoint& generateMove(int color);
	boost::asio::io_service io_service;
	TCPServer tcp_server;
private:
	SocketVector sockets;
	GTPEngine gtp;
};

#endif /* MASTERSERVER_H_ */
