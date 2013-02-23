#include <iostream>
#include "Config.h"

extern int LogLevel;
class Log
{
public:
  //None/ERROR by default, unless otherwise overwritten by program arguments

  inline Log(int level)
  {

  };
  inline ~Log()
  {
  };

  //Normal output
  inline static void Out(std::string message)
  {
    if(LogLevel > DEBUG)
      std::cout<<message<<std::endl;
    //TODO: Implement log to file or other
  }
  //Debugging output. Note inclusion of file and line. __FILE__ and __LINE__ will be used
  inline static void Verbose(std::string message, std::string file, int line)
  {
    if(LogLevel > DEBUG)
      std::cout<<file<<":"<<line<<" - "<<message<<std::endl;
    //TODO: Implement log to file or other
  }

  //Debugging output. Note inclusion of file and line. __FILE__ and __LINE__ will be used
  inline static void Deb(std::string message, std::string file, int line)
  {
    if(LogLevel >= DEBUG)
      std::cout<<file<<":"<<line<<" - "<<message<<std::endl;
    //TODO: Implement log to file or other
  }

  //Erronous output. Note inclusion of file and line. __FILE__ and __LINE__ will be used
  inline static void Err(const std::string message, const char* file, const int line)
  {
    std::cerr<<"Error in "<<file<<":"<<line<<" - "<< message<<"\n\n";
    //TODO: Implement log to file or other
  }
  inline static void In(std::string message)
  {
    //TODO: Implement log to file or other
  }
};
