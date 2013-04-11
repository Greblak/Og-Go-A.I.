#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include "Log.h"

class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
public:

	typedef boost::shared_ptr<TCPConnection> pointer;

	inline static pointer create(boost::asio::io_service& io_service)
	{
		return pointer(new TCPConnection(io_service));
	}

	inline boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}

	inline void start()
	{
	}
private:
	inline TCPConnection(boost::asio::io_service& io_service) : socket_(io_service)
	{
	}

	inline void handle_write()
	{
//		std::cout<<"Handle write called";
	}

	boost::asio::ip::tcp::socket socket_;
};


class TCPServer
{
public: 
	inline TCPServer(
			boost::asio::io_service& io_service,
			int port,
			boost::function<void (boost::shared_ptr<TCPConnection>)> acceptCallback):

			acceptor_(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
			acceptCallback(acceptCallback)
	{
		prepareAccept();
	}

private:
	boost::function<void (boost::shared_ptr<TCPConnection>)> acceptCallback;
	inline void prepareAccept()
	{
		TCPConnection::pointer newConn = TCPConnection::create(acceptor_.get_io_service());
		acceptor_.async_accept(newConn->socket(), boost::bind(&TCPServer::doAccept, this, newConn, boost::asio::placeholders::error));

		LOG_VERBOSE<<"Ready for new connection"<<std::endl;
	}

	inline void doAccept(TCPConnection::pointer newConn, const boost::system::error_code& error)
	{
		if(!error)
		{
			newConn->start();
			LOG_VERBOSE<<"Opened connection from: "<<newConn->socket().remote_endpoint().address().to_string()<<std::endl;
			acceptCallback((*newConn).shared_from_this());
		}

		prepareAccept();
	}

	boost::asio::ip::tcp::acceptor acceptor_;
};


#endif //__TCPSERVER_H__
