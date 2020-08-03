#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <vector>

#include <QMap>
#include <QLabel>
#include <QWidget>
#include <QKeyEvent>
#include <QMainWindow>
#include <QPushButton>
#include <QSharedPointer>

#include "Game.h"
#include "Board.h"
#include "SpriteManager.h"

typedef QSharedPointer<QPushButton> QButtonPtr;

class Window: public QMainWindow
{
	Q_OBJECT

public:
	explicit Window(const std::string& title, Game* game, QWidget* parent = 0,
					int width=640, int height=480);
	~Window();

	// Accessors
	void open(void);
	bool isOpen(void) const;

	// Input accessors
	std::vector<std::string> getButtonInput(void);
	std::vector<int> getKeyboardInput(void);

	// Mutators
	void setTitle(const std::string& title);
	void setSize(int height, int width, bool fixed=true);
	void loadSprites(const std::string& sprites);

	void render(void);

protected:
	// override to catch and handle Qt events
	void keyPressEvent(QKeyEvent* event) override;
	bool event(QEvent* event) override;

	// draw methods
	void paintEvent(QPaintEvent*) override;
	void _drawBoard(QPainter& painter);
	void _drawNextBlock(QPainter& painter);

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

	// GUI text
	void _initializeLabels(void);
	void _setLabelValues(void);
	QWidget _labelPane;
	QLabel _level;
	QLabel _score;
	QLabel _highScore;
	QLabel _nextBlock;

	QWidget _buttonPane;
	QMap<std::string, QButtonPtr> _btns;
	std::vector<std::string> _btnPressed;
};

#endif // WINDOW_H
