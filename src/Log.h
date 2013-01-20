#include <iostream>

class Log
{
public:
	Log(void);
	~Log();

	inline static void Out(std::string message)
	{
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