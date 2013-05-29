#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <iostream>

class Exception
{
public:
	inline Exception(std::string str):str(str) ///< Create new exception
	{

	}
	~Exception();
	inline std::string getMessage() ///< Get error message from exception
	{
		return str;
	}

	/* data */
private:
	const std::string str; ///< Stores error message
};

#endif //#ifndef __EXCEPTION_H__
