/** @file */ 

#ifndef __GTPENGINE_H__
#define __GTPENGINE_H__
#include <string>
#include "GoGame.h"

const int GTP_OFFSET_UPPERCASE_A = 65; //Makes col A 0-indexed
const int GTP_OFFSET_I = 8; //Offset to handle the missing I
const int GTP_OFFSET_LOWERCASE_A = 97; //Makes col a 0-indexed
const int 	GTP_OFFSET_NUM = 48; //Makes row 1 0-indexed

//List of supported commands. Comments state supported arguments
const std::string GTP_ACK_RESPONSE = "= 1\n"; ///< Default ACK
const std::string GTP_CMD_PLAY = "play"; ///< GTP Play command. Arguments: [color] [position]
const std::string GTP_CMD_BOARDSIZE = "boardsize"; ///< GTP Boardsize command
const std::string GTP_CMD_CLEAR_BOARD = "clear_board"; ///< GTP clear board command 
const std::string GTP_CMD_NAME = "name"; ///< GTP name command. Returns name of the AI program
const std::string GTP_CMD_VERSION = "version"; ///< GTP version command. Returns the version of the AI program
const std::string GTP_CMD_PROTOC_VERS = "protocol_version"; ///< GTP protocol version. Should return 2
const std::string GTP_CMD_LIST_CMDS = "list_commands"; ///< GTP list commands. Should return a newline separated list of supported commands
const std::string GTP_CMD_GENMOVE = "genmove"; ///< GTP Genmove command. Called when the GTP client requests a move. Takes color as argument. 
const std::string GTP_CMD_FINAL_SCORE = "final_score"; ///< GTP final score. Used by the GTP client to get the end score for a game
const std::string GTP_CMD_SHOW_BOARD = "showboard"; ///< GTP showboard command. Free return style, shows the state of the board in ASCII format
const std::string GTP_CMD_TIME_SETTINGS = "time_settings"; ///< GTP time settings. Syntax of arguments: [Main time] [Byoyomi time] [Byoyomi stones to be played]
const std::string GTP_CMD_TIME_LEFT = "time_left"; ///< GTP time left. Similar to time_settings but excludes main time. Argument syntax: [Byoyomi time] [Byoyomi stones to be played]
const std::string GTP_CMD_QUIT = "quit"; ///< Tells the program to exit normally.


/*
 * Handles all input and output of GTP commands through console
 */
class GTPEngine
{
private:

	int commandNum;
public:
	GTPEngine(void);
	~GTPEngine();
	void parseList(std::string input); ///< Used to parse a list of GTP commands. Splits them and calls parse on each single command.
	std::vector<std::string> parse(std::string input); ///< Handles user input and sends command to member game.


	const int ColumnStringToInt(std::string str) const ; ///< Converts the column names A-T / a-t to 0-18. Omits the I/i. Capable of taking entire posision string
	static const char ColumnIntToString(int n) ; ///< Converts 0-18 to a-t


	const int RowStringToInt(std::string str) const ; ///< Converts a string containing the row number to an int. Can take the entire position string.
	static const int RowIntToString(int n)  ; ///< Converts row int to row character

	int ColorFromString(std::string str) const; ///< Converts a string describing stone color to respective enum


	static const std::string generateGTPString(GoBoard* board); ///< Generates a string of GTP commands to set up a given board state.
	GoGame* game;
};

#endif //#ifndef __GTPENGINE_H__
