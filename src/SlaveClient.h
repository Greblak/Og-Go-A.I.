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
const int SLAVE_MASTERWAIT_TIME = 5; //Seconds to wait before attempting to reconnect to master.

class SlaveClient {
public:
	SlaveClient(std::string ip, int port);
	virtual ~SlaveClient();
	void run(); ///< Start the slave
	void asyncReadCallback(boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf); ///< Asynchronous callback for reads. 
private:
	std::string masterIP; ///< Ip address to master
	int masterPort; ///< Port to master
	void initSocket(); ///< Start a socket object
	boost::asio::io_service io_service; ///< io_service object
	boost::asio::ip::tcp::socket socket; ///< socket object
	EGTPEngine egtp; ///< EGTP engine object
};

#endif /* SLAVECLIENT_H_ */
