#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>

const std::string PROGRAM_NAME = "Og-Go Random Play A.I."; ///< Used by gtp protocol
const std::string PROGRAM_VERSION = "0.1"; ///< Used by gtp protocol
const std::string PROGRAM_GTP_VERSION = "2"; ///< Used by gtp protocol

const int BOARD_DEFAULT_SIZE = 19; ///< Default board size used unless otherwise stated
const int BOARD_MAX_SIZE = 19; ///< Maximum supported board size
const int BOARD_MINIMUM_SIZE = 2; ///< Minimum valid board size

//
//int BOARD_BOTTOM_LEFT = 0; ///< Default corner values.
//int BOARD_BOTTOM_RIGHT = 18;  ///< Default corner values.
//int BOARD_TOP_LEFT = 342;  ///< Default corner values.
//int BOARD_TOP_RIGHT = 360;  ///< Default corner values.

const int NO_KO_POINT = -1; ///< Defines the KO position. -1 when ko rule does not apply, or no ko point set.



/*
 * Sets the logging levels for the Log-class. ERROR by default.
 */
enum LoggingLevels
{
        PRINT = 0,
	ERROR,
	DEBUG,
	VERBOSE
};

#endif //#ifndef __CONFIG_H__

#ifndef DEBUG_MODE
#define DEBUG_MODE
#endif

