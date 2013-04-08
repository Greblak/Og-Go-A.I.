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
#include <boost/array.hpp>
#include "TCPServer.h"
#include "GTPEngine.h"
#include "UpperConfidence.h"



const int MASTER_BUFFER_MAX_SIZE = 1024;


typedef std::vector<boost::asio::ip::tcp::socket*> SocketVector;
typedef std::vector< boost::array<char, MASTER_BUFFER_MAX_SIZE>* > SocketReadVector;

class MasterServer {
public:
	MasterServer(int port);
	void run();
	virtual ~MasterServer();
	void newConnection(boost::asio::ip::tcp::socket* socket);
	void doHandshake(boost::asio::ip::tcp::socket& socket);
	void write(boost::asio::ip::tcp::socket& socket, const std::string str);
	void writeAll(const std::string str);
	const GoPoint generateMove(int color);
	void genmoveReadCallback();
	boost::asio::io_service io_service;
	TCPServer tcp_server;
private:
	SocketVector sockets;
	SocketReadVector socketreads;
	GTPEngine gtp;
	std::vector<UCBrow> ucbTable;
};

#endif /* MASTERSERVER_H_ */
