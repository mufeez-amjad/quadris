#ifndef GAME_H
#define GAME_H

#include <string>

#include "../generic/Subject.h"

class Game: public Subject
{
public:
	Game();
	~Game();
	void update(const std::string& command);
	bool is_running(void) const;

private:
	bool _running;
};

#endif // GAME_H
