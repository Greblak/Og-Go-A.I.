/*
 * SlaveClient.cpp
 *
 *  Created on: Apr 6, 2013
 *      Author: rune
 */
#include <boost/regex.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/bind.hpp>
#include "EGTPEngine.h"
#include "SlaveClient.h"
#include "Log.h"
#include "Exception.h"

SlaveClient::SlaveClient(std::string ip, int port):socket(io_service),masterIP(ip),masterPort(port),egtp(io_service) {
  initSocket();
}

SlaveClient::~SlaveClient() 
{
  socket.close();
}

void SlaveClient::initSocket()
{
  std::string ip = masterIP;
  int port = masterPort;
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
	{
	  std::cerr << error.message() << std::endl;
	  sleep(5);
	  initSocket();
	}
    }
  catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
}

void SlaveClient::run()
{
  std::cout<<"Starting slave"<<std::endl;
  try
    {

      boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf = new boost::array<char,SLAVE_BUFFER_MAX_SIZE>();
      buf->fill('\0');
      socket.async_read_some(boost::asio::buffer(*buf),boost::bind(&SlaveClient::asyncReadCallback,this,buf));
      io_service.run();
    }
  catch (std::exception& e)
    {
      std::cerr <<"Exception from STD/Boost: "<< e.what() << std::endl;
      std::cerr <<"Attempting to recover. Restarting slave"<<std::endl;
      io_service.stop();
      io_service.reset();
      initSocket();
      this->run();
    }
  catch (Exception& e)
    {
      std::cerr <<"Exception from Og-go AI: "<< e.getMessage() << std::endl;
    }
}



void SlaveClient::asyncReadCallback(boost::array<char, SLAVE_BUFFER_MAX_SIZE>* buf)
{
  size_t bufsize = strlen(buf->data());
  
  boost::regex pattern ("^[A-Za-z]",boost::regex_constants::icase|boost::regex_constants::perl);

  if(bufsize>0)
    {
     
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
	  std::vector<std::string> args;
	  boost::split(args, cmds[0],boost::is_any_of( " " ));
	  int commandNumber = 0;
	  commandNumber = atoi(args[0].c_str());
	  if(commandNumber != 0) //First argument is the command number
	    {
	      args.erase(args.begin());
	      cmds[0] = boost::algorithm::join(args," ");
	    }
	  
	  
	  for(std::vector<std::string>::iterator it = cmds.begin(); it != cmds.end(); ++it)
	    {
	      if(strlen((*it).c_str()) == 0 || !boost::regex_search ((*it),pattern, boost::regex_constants::format_perl))
		continue;
	      std::string output = egtp.parse(*it);
	      if(strlen(output.c_str()) > 0)
		output.substr(2);
	      std::stringstream ss;
	      ss<<"="<<commandNumber<<" "<<output;
	      std::cout<<"Sending back: "<<ss.str()<<std::endl;
	      socket.write_some(boost::asio::buffer(ss.str()));
	    }
	}
      socket.async_read_some(boost::asio::buffer(*buf),boost::bind(&SlaveClient::asyncReadCallback,this,buf));
    }
}
