#ifndef VIEW_H
#define VIEW_H

#include "Game.h"
#include "Controller.h"

#include <string>

class View: public Observer
{
public:
	View(Game* _game, Controller* controller);
	virtual ~View();

	virtual void pollInput(void) = 0;
	virtual bool isOpen(void) const = 0;
	virtual void launch(void);

protected:
	Game* _game;
	Controller* _controller;
	bool _subscribed;
};

#endif // VIEW_H
