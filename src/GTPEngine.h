#ifndef __GTPENGINE_H__
#define __GTPENGINE_H__
#include <string>
#include "GoGame.h"

const int GTP_OFFSET_UPPERCASE_A = 65; //Makes col A 0-indexed
const int GTP_OFFSET_I = 8; //Offset to handle the missing I
const int GTP_OFFSET_LOWERCASE_A = 97; //Makes col a 0-indexed
const int GTP_OFFSET_NUM = 49; //Makes row 1 0-indexed


class GTPEngine
{
	GoGame* game;
public:
	GTPEngine(void);
	~GTPEngine();

	void parse();

	const int ColumnStringToInt(std::string str) const ;
	const char ColumnIntToString(int n) const ;

	const int RowStringToInt(std::string str) const ;
	const int RowIntToString(int n) const ;

	const int ColorFromString(std::string str) const;
};

#endif //#ifndef __GTPENGINE_H__