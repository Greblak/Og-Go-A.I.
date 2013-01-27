#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <iostream>

class Exception
{
public:
	inline Exception(std::string str):str(str)
	{

	}
	~Exception();
	inline std::string getMessage()
	{
		return str;
	}

	/* data */
private:
	const std::string str;
};

#endif //#ifndef __EXCEPTION_H__