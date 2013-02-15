#include <iostream>

class Log
{
public:
	 //None/ERROR by default, unless otherwise overwritten by program arguments

	inline Log(void)
	{

	};
	inline ~Log()
	{
	};

	inline static void Out(std::string message)
	{
		if(LogLevel > DEBUG)
			std::cout<<message<<"\n\n";
		//TODO: Implement log to file or other
	}
	inline static void Err(std::string message)
	{
		std::cerr<<message<<"\n\n";
		//TODO: Implement log to file or other
	}
	inline static void In(std::string message)
	{
		//TODO: Implement log to file or other
	}
};