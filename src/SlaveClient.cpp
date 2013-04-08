/*
 * SlaveClient.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include "EGTPEngine.h"
#include "SlaveClient.h"
#include "Log.h"

SlaveClient::SlaveClient(std::string ip, int port):socket(io_service) {
	LOG_VERBOSE << "Attempting to connect to "<<ip<<":"<<port;

	try
	{;

	boost::asio::ip::tcp::resolver resolver(io_service);
	boost::asio::ip::tcp::resolver::query query(ip, "1919");
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
	boost::asio::ip::tcp::resolver::iterator end;


	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error && endpoint_iterator != end)
	{
		socket.close();
		socket.connect(*endpoint_iterator++, error);
	}
	if (error)
		throw boost::system::system_error(error);


	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

}

SlaveClient::~SlaveClient() {
	socket.close();
}

void SlaveClient::run()
{

	//	    socket.write_

	EGTPEngine egtp;
	while(true)
	{
		boost::array<char, SLAVE_BUFFER_MAX_SIZE> buf;
		boost::system::error_code error;
		size_t len = socket.read_some(boost::asio::buffer(buf), error);
		if (error)
		{
			socket.close();
			return;
		}
		size_t bufsize = strlen(buf.data());
		std::string input = std::string(buf.data(),bufsize);
		std::string output = egtp.parse(input);

		if(std::string(buf.data(),bufsize) == EGTP_HANDSHAKE)
		{
			socket.write_some(boost::asio::buffer(GTP_ACK_RESPONSE));
		}
	}
}

