/*
 * SlaveClient.h
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#ifndef SLAVECLIENT_H_
#define SLAVECLIENT_H_

#include <boost/asio.hpp>

const int SLAVE_BUFFER_MAX_SIZE = 1024;

class SlaveClient {
public:
	SlaveClient(std::string ip, int port);
	virtual ~SlaveClient();
	void run();
	void asyncReadCallback(boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf);
private:
	std::string masterIP;
	int masterPort;
	void initSocket(); 
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;
	EGTPEngine egtp;
};

#endif /* SLAVECLIENT_H_ */
