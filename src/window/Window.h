#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

#include <QMap>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QSharedPointer>
#include <QKeyEvent>

#include "../game/Game.h"
#include "../blocks/Board.h"
#include "../graphics/SpriteManager.h"

typedef QSharedPointer<QPushButton> QButtonPtr;

class Window: public QMainWindow
{
	Q_OBJECT

public:
	explicit Window(const std::string& title, Game* game, QWidget* parent = 0, int width=640, int height=480);
	~Window();

	void open(void);
	bool isOpen(void) const;

	// Mutators
	void setTitle(const std::string& title);
	void setSize(int height, int width, bool fixed=true);
	void loadSprites(const std::string& sprites);

	// Input accessors
	std::vector<std::string> getButtonInput(void);
	std::vector<int> getKeyboardInput(void);

	void render(void);

protected:
	// override to catch and handle Qt events
	void keyPressEvent(QKeyEvent* event) override;
	bool event(QEvent* event) override;

	// draw methods
	void paintEvent(QPaintEvent*) override;
	void _drawBoard(QPainter& painter);

private:
	bool _open;
	int _width, _height;
	std::string _title;

	Game* _game;

	// Cached assets
	std::unique_ptr<SpriteManager> _sprites;

	// Keyboard input
	std::vector<int> _keyPressed;

	// GUI controls
	void _initializeButtons(void);
	void _positionButtons(void);
	void _onButtonPress(std::string command);
	QWidget _buttonPane;
	QMap<std::string, QButtonPtr> _btns;
	std::vector<std::string> _btnPressed;
};

#endif // WINDOW_H
