#ifndef WINDOW_H
#define WINDOW_H

#include "../blocks/Board.h"

#include <memory>
#include <string>

#include <QMainWindow>
#include <QWidget>

class Window: public QMainWindow
{
	Q_OBJECT

public:
	explicit Window(const std::string& title, QWidget* parent = 0, int width=640, int height=480);
	~Window();

	void open(void);
	bool isOpen(void) const;
	void setTitle(const std::string& title);
	void setSize(int height, int width);

	void render(void);

protected:
	// override to catch and handle Qt events
	bool event(QEvent* event) override;
	bool confirmClose(void);

	void paintEvent(QPaintEvent*) override;

private:
	bool _open;
	int _width, _height;
	std::string _title;
	Sprite2D _card;
	Board _board;
};

#endif // WINDOW_H
