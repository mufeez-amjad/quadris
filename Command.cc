#include "Command.h"

static const std::string default_message = "no message";

Command::Command(std::string s, CommandType type, bool silent)
	: type{ type }, silent{ silent }, message{ s }
{

}

Command::Command(CommandType type, bool silent)
	: type{ type }, silent{ silent }, message{ default_message }
{

}

Command::Command()
	: type{ CommandType::UNDEFINED_COMMAND }, silent{ false },
	message{ default_message }
{

}
