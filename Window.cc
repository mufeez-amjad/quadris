#include "Window.h"
#include "Arguments.h"

#include <iostream>

#include <QEvent>
#include <QPainter>
#include <QAction>
#include <QFont>

#define BUTTON_COUNT 10
static const std::string buttonLabels[BUTTON_COUNT] = {
	"left", "right", "down", "drop", "clockwise", "counter clockwise",
	"level up", "level down", "restart", "hint"
};

static const char* buttonStyle =
"font-family: Arial;"
"font-size: 14px;"
"color: rgb(65,255,0);"
"border-width: 1px;"
"border-style: solid;"
"border-color: rgb(65,255,0);"
"padding-left: 5px;"
"padding-right: 5px;"
"padding-top: 2px;"
"padding-bottom: 2px;"
"min-width: 40px;";

Window::Window(const std::string& title, Game* game, QWidget* parent,
               int width, int height)
	: QMainWindow(parent), _open{ false }, _game{ game },
	_labelPane{ this }
{
	this->setStyleSheet("background-color:black");
	this->setTitle(title);
	this->setSize(width, height);
	if (Arguments::has("-bonus"))
		this->_initializeButtons();
	this->_initializeLabels();
}

Window::~Window()
{

}

void Window::loadSprites(const std::string& sprites)
{
	this->_sprites = std::make_unique<SpriteManager>(sprites);
}

// precondition: Game::_board must be initialized
void Window::_initializeButtons(void)
{
	// set button pane position

	this->_buttonPane.setParent(this);

	int paneX = this->_game->getBoard().getCellSize() * 12;
	int paneY = this->_game->getBoard().getCellSize() * 8;

	this->_buttonPane.move(paneX, paneY);
	this->_buttonPane.resize(this->_width - paneX, this->_height - paneY);

	// populate the button pane
	for (auto & label : buttonLabels) {
		QString buttonText = QString::fromStdString(label);
		QButtonPtr b(new QPushButton(buttonText, &this->_buttonPane));
		this->_btns.insert(label, b);
		b->setStyleSheet(buttonStyle);
		// disable selecting buttons with keyboard
		b->setFocusPolicy(Qt::NoFocus);

		// register button callback
		// raw pointer context needed for Qt to manage receiver lifespan
		QPushButton* context = b.data();
		connect(context, &QPushButton::clicked, context, [context, this] {
			if (context != nullptr) {
				std::string command = context->text().toStdString();
				this->_onButtonPress(command);
			}
		});
	}
	this->_positionButtons();
}

// set button positions
// precondition: _btns must be initialized
void Window::_positionButtons(void)
{
	int row_space = 33;
	this->_btns["restart"]->move(0, row_space * 0);
	this->_btns["counter clockwise"]->move(0, row_space * 1);
	this->_btns["clockwise"]->move(145, row_space * 1);
	this->_btns["left"]->move(0, row_space * 2);
	this->_btns["right"]->move(60, row_space * 2);
	this->_btns["down"]->move(0, row_space * 3);
	this->_btns["drop"]->move(0, row_space * 4);
	this->_btns["level down"]->move(0, row_space * 5);
	this->_btns["level up"]->move(95, row_space * 5);
	this->_btns["hint"]->move(0, row_space * 6);
}

void Window::_onButtonPress(std::string command)
{
	// queue button pressed for View to relay to controller
	this->_btnPressed.push_back(command);
}

void Window::_initializeLabels(void)
{
	// set button pane position
	int paneX = this->_game->getBoard().getCellSize() * 12;
	int paneY = this->_game->getBoard().getCellSize() * 5;

	this->_labelPane.move(paneX, 0);
	this->_labelPane.resize(this->_width - paneX, paneY);

	std::vector<QLabel*> labels = { &this->_level, &this->_score,
	                                &this->_highScore, &this->_nextBlock };

	int i = 0;
	QFont font("Consolas", 14);
	for (auto& l : labels) {
		l->setFont(font);
		l->setStyleSheet("color: rgb(65,255,0);");
		l->setParent(&this->_labelPane);
		l->move(0, i*30 + 15);
		l->setMinimumWidth(300);
		++i;
	}
	this->_nextBlock.setText("Next Block:");
	this->_setLabelValues();
}

void Window::_setLabelValues(void)
{
	auto level = QString::number(this->_game->getLevel()).prepend("Level: ");
	auto score = QString::number(this->_game->getScore()).prepend("Score: ");
	auto hscore = QString::number(this->_game->getHighScore());
	hscore.prepend("High Score: ");
	this->_level.setText(level);
	this->_score.setText(score);
	this->_highScore.setText(hscore);
}

std::vector<std::string> Window::getButtonInput(void)
{
	std::vector<std::string> buttonInput = this->_btnPressed;
	this->_btnPressed.clear();
	return buttonInput;
}

std::vector<int> Window::getKeyboardInput(void)
{
	std::vector<int> keyboardInput = this->_keyPressed;
	this->_keyPressed.clear();
	return keyboardInput;
}

void Window::render(void)
{
	// trigger paintEvent by updating widget
	QWidget::update();
	this->_setLabelValues();
}

void Window::paintEvent(QPaintEvent* event)
{
	if (event != nullptr) {
		QPainter painter(this);
		this->_drawNextBlock(painter);
		this->_drawBoard(painter);
	}
}

void Window::_drawBoard(QPainter& painter)
{
	// render board
	const Board& board = this->_game->getBoard();
	float cellSize = board.getCellSize();
	for (auto i = board.begin(); i != board.end(); ++i) {
		Cell c = *i;
		float x = c.get_x() * cellSize;
		// display y values have top at y = 0
		float y = (17.0f - c.get_y()) * cellSize;

		QRectF target = QRectF(x, y, cellSize, cellSize);
		auto sprite = this->_sprites->getSprite(c.getSprite());
		QPixmap pm = sprite->getData();
		painter.drawPixmap(target, pm, pm.rect());
	}

	// render current block
	std::shared_ptr<Block> currentBlock = board.getCurrentBlock();
	if (currentBlock != nullptr) {
		for (auto& c : currentBlock->getCells()) {
			float x = c->get_x() * cellSize;
			// display y values have top at y = 0
			float y = (17.0f - c->get_y()) * cellSize;

			QRectF target = QRectF(x, y, cellSize, cellSize);
			auto sprite = this->_sprites->getSprite(c->getSprite());
			QPixmap pm = sprite->getData();
			painter.drawPixmap(target, pm, pm.rect());
		}
	}
}

void Window::_drawNextBlock(QPainter& painter)
{
	const Board& board = this->_game->getBoard();
	float cellSize = board.getCellSize();
	// render next block
	float dx = cellSize * 17.0f;
	float dy = cellSize* 5.0f;
	std::shared_ptr<Block> next = board.getNextBlock();

	if (next == nullptr)
		return;

	next->blockSpace(true);
	for (auto& c : next->getCells()) {
		float x = (c->get_x() * cellSize) + dx;
		float y = ((next->yMax() - c->get_y()) * cellSize) + dy;
		QRectF target = QRectF(x, y, cellSize, cellSize);
		auto sprite = this->_sprites->getSprite(c->getSprite());
		QPixmap pm = sprite->getData();
		painter.drawPixmap(target, pm, pm.rect());
	}
	next->blockSpace(false);
}

// handle Qt events
bool Window::event(QEvent* event)
{
	switch (event->type()) {
		case QEvent::Close:
			this->_open = false;
			return true;
		case QEvent::Resize:
			this->_width = this->width();
			this->_height = this->height();
			return true;
		default:
			// do not intercept these events
			return QMainWindow::event(event);
	}
}

void Window::keyPressEvent(QKeyEvent* event)
{
	if (Arguments::has("-bonus"))
		this->_keyPressed.push_back(event->key());
}

void Window::open(void)
{
	this->setVisible(true);
	this->_open = true;
}

void Window::setTitle(const std::string& title)
{
	this->_title = title;
	this->setWindowTitle(title.c_str());
}

void Window::setSize(int width, int height, bool fixed)
{
	this->_width = width;
	this->_height = height;
	if (fixed)
		this->setFixedSize(width, height);
	else
		this->setSize(width, height);
}

bool Window::isOpen(void) const
{
	return _open;
}
