#include <iostream>
#include "Config.h"
#include <sstream>
#include <fstream>


#define LOG_VERBOSE Log(VERBOSE).Verbose(__FILE__,__LINE__)
#define LOG_DEBUG Log(DEBUG).Deb(__FILE__,__LINE__)
#define LOG_ERROR Log(ERROR).Err(__FILE__,__LINE__)
#define LOG_OUT Log(ERROR).Out()

extern int LogLevel;
class Log
{
private:
  std::stringstream ss;
  int level; ///< ERROR by default, unless otherwise set by execution flags
  std::ofstream logfile; ///< File to use for output
public:


  inline Log(int l):level(l)
  {
  };

  /*!
   * Log messages are output to file and console when Log instance is destroyed.
   * Log should ALWAYS be put on the stack and not on the heap. Log messages will appear when it goes out of scope.
   */
  inline ~Log()
  {
    //    ss<<std::endl;
    if(LogLevel>=level)
      {
        if(level >ERROR)
          std::cerr<< ss.str() << "\n";
        else
          std::cout << ss.str() << "\n\n";
      }


    logfile.open("Debug.log",std::ios::app);
    logfile << "["<<level<<"] " << ss.str() << std::endl;
    logfile.close();
  };


  /*!
   * Verbose messages. Trivial messages
   */
  inline std::stringstream& Verbose(std::string file, int line)
  {
    if(LogLevel >= VERBOSE)
      ss<<"[VER]"<<file<<":"<<line<<" - ";
    return ss;
  }

  /*!
   * Used for messages related to debugging
   */
  inline std::stringstream& Deb(std::string file, int line)
  {
    if(LogLevel >= DEBUG)
      ss<<"[DEB]"<<file<<":"<<line<<" - ";
    return ss;
  }

  /*!
   * Serious error messages
   */
  inline std::stringstream& Err(std::string file, int line)
  {
    if(LogLevel >= ERROR)
      ss<<"?"<<file<<":"<<line<<" - ";
    return ss;
  }

  /*!
   * All messages that go to console. These are not marked with __FILE__ and __LINE__ for GTP compliancy.
   */
  inline std::stringstream& Out()
  {
    return ss;
  }
};
