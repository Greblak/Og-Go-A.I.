/*
 * MasterServer.h
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#ifndef MASTERSERVER_H_
#define MASTERSERVER_H_

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include "TCPServer.h"
#include "GTPEngine.h"
#include "UpperConfidence.h"



const int MASTER_BUFFER_MAX_SIZE = 1024;




class MasterServer :public boost::enable_shared_from_this<MasterServer>
{
public:
	MasterServer(int port);
	void run();
	virtual ~MasterServer();
	void newConnection(boost::shared_ptr<TCPConnection> conn);
	void doHandshake(boost::shared_ptr<TCPConnection> conn);
	void write(boost::shared_ptr<TCPConnection> conn, const std::string str);
	void writeHandler();
	void writeAll(const std::string str);
	const GoPoint generateMove(int color);
	void genmoveReadCallback(boost::shared_ptr<TCPConnection> conn, boost::array<char, 1024>* buf);
	void setTimeout(int seconds);

	boost::asio::io_service io_service;
	TCPServer tcp_server;
private:
	void checkDeadConnections();
	void ioServiceStarter();
	
	GTPEngine gtp;
	std::vector<UCBrow> ucbTable;
	bool writingToUcbTable;
	int genmoveResponses;
	bool genmoveResponseWait;
	int genmoveTimeoutMilliSec;
	int commandNumber;
	GoPoint bestMove;
};

#endif /* MASTERSERVER_H_ */
