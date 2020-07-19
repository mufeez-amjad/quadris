#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include "View.h"
#include "../window/Window.h"

#include <string>
#include <future>

class GraphicsView : public View
{
public:
	GraphicsView(const std::string& name, Game* game, CommandInterpreter* interpreter, int argc, char** argv);
	void poll_input(void) override;

	void notify(void) const override;

private:
	Window _window;
};

#endif // GRAPHICSVIEW_H
