#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <vector>
#include <algorithm>

class Arguments
{
public:
	static void parse(int argc, char* argv[]);
	static bool has(const std::string& arg);
	static std::string getParam(const std::string& param);

private:
	static std::vector<std::string>::iterator find(const std::string& loc);
	static std::vector<std::string> _args;
};

#endif // ARGUMENTS_H
