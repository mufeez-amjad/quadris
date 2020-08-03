#include "Controller.h"

#include <iostream>

typedef std::lock_guard<std::mutex> thread_lock;

Controller::Controller(Game& game)
	: _game{ game }
{

}

// lock mutex to prevent writing to queue while another thread
// is adding accessing or modifying the queue
void Controller::push(Command c)
{
	thread_lock lk(this->_lock);

	if (c.type == CommandType::UNDEFINED_COMMAND)
		return;

	this->_commandQueue.push(c);
}

// lock mutex to prevent writing to queue while another thread
// is adding accessing or modifying the queue
// this method must be used by views
void Controller::push(std::vector<Command> cs)
{
	thread_lock lk(this->_lock);

	for (auto& c: cs)
		this->_commandQueue.push(c);
}

// lock mutex to prevent the queue from being accessed while
// a flush is in process
void Controller::sendCommands(void)
{
	thread_lock lk(this->_lock);
	while (!this->_commandQueue.empty()) {
		this->_game.update(this->_commandQueue.front());
		this->_commandQueue.pop();
	}
}
