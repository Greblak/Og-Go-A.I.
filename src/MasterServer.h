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
	void run(); ///< Start the server
	virtual ~MasterServer();
	void newConnection(boost::shared_ptr<TCPConnection> conn); ///< Prepare a new connection
	void doHandshake(boost::shared_ptr<TCPConnection> conn); ///< Perform a handshake to confirm validity
	void write(boost::shared_ptr<TCPConnection> conn, const std::string str); ///< Write to a connection
	void writeHandler(); ///< Async write handler. Currently does nothing.
	void writeAll(const std::string str); ///< Write to all current connections
	const GoPoint generateMove(int color); ///< Generate move
	void genmoveReadCallback(boost::shared_ptr<TCPConnection> conn, boost::array<char, 1024>* buf); ///< Callback after calling generateMove
	void setTimeout(int seconds);

	boost::asio::io_service io_service; ///< Boost io service used for asynchronous methods
	TCPServer tcp_server; ///< TCP server backend
private:
	void checkDeadConnections(); ///< Test for dead connections and drop them
	void ioServiceStarter(); ///< Start the io_service
	
	GTPEngine gtp; ///< GTP Engine object
	std::vector<UCBrow> ucbTable; ///< Current ucbtable. Used for MC and UCB results
	bool writingToUcbTable; ///< Mutex to prevent asyncronous operations from writing while another operation is writing to the table
	int genmoveResponses; ///< Number of slaves that has returned a response
	bool genmoveResponseWait; ///< Number of seconds to wait for slaves to return
	int genmoveTimeoutMilliSec; ///< Not used
	int commandNumber; ///< All GTP commands are numbered. This is done to prevent acting on outdated commands
	GoPoint bestMove; ///< Selected best move from AI
};

#endif /* MASTERSERVER_H_ */
