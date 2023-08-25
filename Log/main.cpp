#include <iostream>
#include "Logger.h"

using namespace yazi::utility;


int main()
{
	Logger::instance()->open("./test.log");
	Logger::instance()->level(Logger::DEBUG);
	Logger::instance()->max(200);

	debug("hello world");
	debug("name is %s, age is %d", "jack", 18);
	error("error message");
	info("info message");
	warn("warn message");
	fatal("fatal message");

	return 0; 
}