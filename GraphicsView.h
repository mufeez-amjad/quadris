#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <string>
#include <QApplication>

#include "View.h"
#include "Window.h"

class GraphicsView : public View
{
public:
	GraphicsView(const std::string& name, Game* game, Controller* controller);
	void pollInput(void) override;

	void update(void) override;

	bool isOpen(void) const override;

private:
	void _shutdown(void);

	// Qt application arguments
	int _qtArgCount;
	char** _qtArgs;

	bool _open;
	std::string _name;
	QApplication _app;
	Window _window;
};

#endif // GRAPHICSVIEW_H
