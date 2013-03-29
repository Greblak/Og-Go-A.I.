#ifndef __GTPENGINE_H__
#define __GTPENGINE_H__
#include <string>
#include "GoGame.h"

const int GTP_OFFSET_UPPERCASE_A = 65; //Makes col A 0-indexed
const int GTP_OFFSET_I = 8; //Offset to handle the missing I
const int GTP_OFFSET_LOWERCASE_A = 97; //Makes col a 0-indexed
const int 	GTP_OFFSET_NUM = 48; //Makes row 1 0-indexed

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
	void parseList(std::string input);
	std::vector<std::string> parse(std::string input); ///< Handles user input and sends command to member game.


	const int ColumnStringToInt(std::string str) const ; ///< Converts the column names A-T / a-t to 0-18. Omits the I/i. Capable of taking entire posision string
	static const char ColumnIntToString(int n) ; ///< Converts 0-18 to a-t


	const int RowStringToInt(std::string str) const ; ///< Converts a string containing the row number to an int. Can take the entire position string.
	static const int RowIntToString(int n)  ; ///< Converts row int to row character

	int ColorFromString(std::string str) const; ///< Converts a string describing stone color to respective enum


	static const std::string generateGTPString(GoBoard* board);
	GoGame* game;
};

#endif //#ifndef __GTPENGINE_H__
