#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/function.hpp>
#include "Log.h"

const int SOCKET_DEFAULT_TIMEOUT = 5;
class TCPConnection : public boost::enable_shared_from_this<TCPConnection>
{
 public:

  typedef boost::shared_ptr<TCPConnection> pointer;

  inline static pointer create(boost::asio::io_service& io_service)
  {
    //timeoutTimer(io_service);
    return pointer(new TCPConnection(io_service));
  }


  inline boost::asio::ip::tcp::socket& socket()
  {
    return socket_;
  }

  inline void start()
  {
  }

  inline void resetTimeout()
  {
    LOG_VERBOSE<<"Timeout reset"<<std::endl;
    timeoutTimer.cancel();
  }

  inline void startTimeout(int seconds = SOCKET_DEFAULT_TIMEOUT)
  {
    LOG_VERBOSE<<"Started timer at "<<seconds<<" seconds"<<std::endl;
    timeoutTimer.expires_from_now(boost::posix_time::seconds(seconds));
    timeoutTimer.async_wait(boost::bind(&TCPConnection::handle_timeout,this,_1));
  }

  inline bool is_open()
  {
    return isOpen;
  }

 private:
  bool isOpen;
  boost::asio::deadline_timer timeoutTimer;
  inline TCPConnection(boost::asio::io_service& io_service) : socket_(io_service),timeoutTimer(io_service),isOpen(true)
  {
  }

  inline void handle_write()
  {
    //		std::cout<<"Handle write called";
  }

    inline void handle_timeout(const boost::system::error_code& e)
  {
   
	// Timer expired.
    if(!e)
      {
	std::cout<<"Timeout. Socket dropped."<<std::endl;
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send);
	socket_.close();
	isOpen = false;
      }
    
  }
  
  boost::asio::ip::tcp::socket socket_;
};

typedef std::vector<boost::shared_ptr<TCPConnection> > SocketVector;
typedef std::vector< boost::array<char, 1024>* > SocketReadVector;

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
      LOG_VERBOSE<<"Attempting to create server on port: "<<port<<std::endl;
      prepareAccept();
    }
  SocketVector sockets;
  SocketReadVector socketreads;

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
