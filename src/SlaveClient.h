/*
 * SlaveClient.h
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#ifndef SLAVECLIENT_H_
#define SLAVECLIENT_H_

#include <boost/asio.hpp>

class SlaveClient {
public:
	SlaveClient(std::string ip, int port);
	virtual ~SlaveClient();
	void run();
private:
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket socket;
};

#endif /* SLAVECLIENT_H_ */
