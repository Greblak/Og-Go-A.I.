#include <iostream>
#include "Config.h"
#include <sstream>
#include <fstream>


#define LOG_VERBOSE Log(VERBOSE).Verbose(__FILE__,__LINE__)
#define LOG_DEBUG Log(DEBUG).Deb(__FILE__,__LINE__)
#define LOG_ERROR Log(ERROR).Err(__FILE__,__LINE__)
#define LOG_OUT Log(PRINT).Out()

extern int LogLevel;
class Log
{
private:
  std::stringstream ss;
  int level;
  std::ofstream logfile;
public:
  //None/ERROR by default, unless otherwise overwritten by program arguments

  inline Log(int l):level(l)
  {
  };
  inline ~Log()
  {
    if(LogLevel)
      { 
	std::cout << ss.str()  <<std::endl;
	logfile.open("Debug.log",std::ios::app);
	logfile << ss.str() <<std::endl;
	logfile.close();
	       
      }
  };


  inline std::stringstream& Verbose(std::string file, int line)
    {
      if(LogLevel >= VERBOSE)
        ss<<file<<":"<<line<<" - ";
      return ss;
      //TODO: Implement log to file or other
    }

  inline std::stringstream& Deb(std::string file, int line)
    {
      if(LogLevel >= DEBUG)
        ss<<file<<":"<<line<<" - ";
      return ss;
      //TODO: Implement log to file or other
    }

  inline std::stringstream& Err(std::string file, int line)
    {
      if(LogLevel >= ERROR)
        ss<<file<<":"<<line<<" - ";
      return ss;
      //TODO: Implement log to file or other
    }

  inline std::stringstream& Out()
    {
      return ss;
      //TODO: Implement log to file or other
    }
};
