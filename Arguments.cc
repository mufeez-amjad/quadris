#include "Arguments.h"

std::vector<std::string> Arguments::_args;

void Arguments::parse(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		std::string arg = argv[i];
		Arguments::_args.emplace_back(arg);
	}
}

std::vector<std::string>::iterator Arguments::find(const std::string& loc)
{
	return std::find(Arguments::_args.begin(), Arguments::_args.end(), loc);
}

bool Arguments::has(const std::string& arg)
{
	return Arguments::find(arg) != Arguments::_args.end();
}

std::string Arguments::getParam(const std::string& param)
{
	return *(++Arguments::find(param));
}
