/*
 * PipeCommunication.h
 *
 *  Created on: Mar 27, 2013
 *      Author: rune
 */

#ifndef PIPECOMMUNICATION_H_
#define PIPECOMMUNICATION_H_

#include <string>
#include <sstream>
#include <iostream>
#include "Log.h"
class PipeCommunication {
public:
	static inline std::string readPipe(int pipe)
	{
		char buf;
		std::stringstream ss;
		bool wait = true;
		int timeout = 10;
		while(wait && timeout>0)
		{
			while(read(pipe,&buf,1) > 0 && wait)
			{
				if(buf == '\n')
				{
					wait = false;
					break;
				}
				ss<<buf;
			}
			if(!wait)
				break;
//			std::cout<<"waiting "<<timeout<<std::endl;
			sleep(1);
			timeout-=1;
		}
//		if(timeout == 0)
//			std::cerr<<"Read timed out";
//		std::cout<<"read: "<<ss.str()<<std::endl;
		return ss.str();
	}

	static inline int writePipe(int pipe, std::string message)
	{
		char buf[message.size()];
		for(int i = 0; i<message.size();++i)
			buf[i] = message[i];
		return write(pipe, &buf, sizeof(buf));
	}
};

#endif /* PIPECOMMUNICATION_H_ */
