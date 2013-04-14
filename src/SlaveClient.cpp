/*
 * SlaveClient.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include "EGTPEngine.h"
#include "SlaveClient.h"
#include "Log.h"
#include "Exception.h"

SlaveClient::SlaveClient(std::string ip, int port):socket(io_service) {
  LOG_VERBOSE << "Attempting to connect to "<<ip<<":"<<port;

  try
    {

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

SlaveClient::~SlaveClient() 
{
  socket.close();
}

void SlaveClient::run()
{
  LOG_VERBOSE<<"Starting slave"<<std::endl;
  try
    {
      boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf = new boost::array<char,SLAVE_BUFFER_MAX_SIZE>();
      buf->fill('\0');
      socket.async_read_some(boost::asio::buffer(*buf),boost::bind(&SlaveClient::asyncReadCallback,this,buf));
      io_service.run();
    }
  catch (std::exception& e)
    {
      std::cerr <<"STD EX"<< e.what() << std::endl;
    }
  catch (Exception& e)
    {
      std::cerr <<"PERS EX"<< e.getMessage() << std::endl;
    }
}

void SlaveClient::asyncReadCallback(boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf)
{
  size_t bufsize = strlen(buf->data());
  if(bufsize>0)
    {
      std::cout<<"Got: "<<buf->data()<<std::endl;
      if(std::string(buf->data(),bufsize) == EGTP_HANDSHAKE)
	{
	  LOG_DEBUG<<"Got handshake, approved. Sending response"<<std::endl;
	  socket.write_some(boost::asio::buffer(GTP_ACK_RESPONSE));
	}
      else
	{
	  std::string input = std::string(buf->data(),bufsize);
	  std::cout<<"Got input: "<<input<<std::endl;
	  std::vector<std::string> cmds;
	  boost::split(cmds,input, boost::is_any_of( "\n" ));
	  for(std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
	    {
	      if(strlen((*it).c_str()) == 0)
		continue;
	      std::string output = egtp.parse(*it);
	      std::cout<<"Sending back: "<<output<<std::endl;
	      socket.write_some(boost::asio::buffer(output));
	    }
	}
      socket.async_read_some(boost::asio::buffer(*buf),boost::bind(&SlaveClient::asyncReadCallback,this,buf));
    }
}
