#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

const std::string PROGRAM_NAME = "Og-Go Random Play A.I.";
const std::string PROGRAM_VERSION = "0.1";
const std::string PROGRAM_GTP_VERSION = "2";

const int BOARD_DEFAULT_SIZE = 19;
const int BOARD_MAX_SIZE = 19;
const int BOARD_MINIMUM_SIZE = 2;

const int BOARD_BOTTOM_LEFT = 0;
const int BOARD_BOTTOM_RIGHT = 18;
const int BOARD_TOP_LEFT = 342;
const int BOARD_TOP_RIGHT = 360;

enum LoggingLevels
{
	ERROR = 0,
	DEBUG,
	VERBOSE
};

#endif //#ifndef __CONFIG_H__
#ifndef DEBUG_MODE
#define DEBUG_MODE
#endif

