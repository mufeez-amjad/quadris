#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <queue>
#include <vector>
#include <string>
#include <mutex>

#include "Game.h"
#include "Command.h"

class Controller
{
public:
	Controller(Game& game);

	void push(Command command);
	void push(std::vector<Command> commands);
	void sendCommands(void);

private:
	std::mutex _lock;
	Game& _game;
	std::queue<Command> _commandQueue;
};

#endif // CONTROLLER_H
