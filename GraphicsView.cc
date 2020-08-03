#include "GraphicsView.h"

#include <iostream>
#include <unordered_map>

#include "Command.h"

static const std::unordered_map<std::string, CommandType> buttonCommandMap =
{
	{"left", CMD::LEFT}, {"right", CMD::RIGHT}, {"drop", CMD::DROP},
	{"down", CMD::DOWN}, {"clockwise", CMD::CLOCKWISE},
	{"counter clockwise", CMD::COUNTERCLOCKWISE},
	{"level up", CMD::LEVELUP}, {"level down", CMD::LEVELDOWN},
	{"restart", CMD::RESTART}, {"hint", CMD::HINT}
};

static const std::unordered_map<int, CommandType> keyCommandMap = {
	{Qt::Key::Key_A, CMD::LEFT}, {Qt::Key::Key_S, CMD::DOWN},
	{Qt::Key::Key_D, CMD::RIGHT},
	{Qt::Key::Key_Left, CMD::COUNTERCLOCKWISE},
	{Qt::Key::Key_Right, CMD::CLOCKWISE}, {Qt::Key::Key_Space, CMD::DROP}
};

// prevent QtWarningMsg from writing to console and ruining ConsoleView's
// display.
//
// Qt documentation reference code was modified to implement this function for
// Qt message handling. The reference code used is from:
// https://doc.qt.io/qt-5/qtglobal.html#qInstallMessageHandler
void messageOutput(QtMsgType type,
                   const QMessageLogContext &context,
                   const QString &msg)
{
	std::string message;
	switch (type) {
		case QtCriticalMsg:
		case QtFatalMsg:
			message = msg.toStdString();
			std::cerr << "Qt ERROR: " << message
			          << " (" << context.file << ": " << context.line
			          << ", " << context.function << ")\n";
			break;
		default:
			break;
	}
}

GraphicsView::GraphicsView(const std::string& name, Game* game,
	                       Controller* controller)
	: View{ game, controller }, _qtArgCount{ 0 }, _qtArgs{ nullptr },
	_open{ false }, _name{ name }, _app(_qtArgCount, _qtArgs),
	_window{ name, game, 0, 550, (int) game->getBoard().getCellSize() * 18 }
{
	qInstallMessageHandler(messageOutput);
	// load quadris cell sprites
	this->_window.loadSprites("./sprites.txt");
	this->_window.open();
	this->_open = true;
}

void GraphicsView::update(void)
{
	if (this->_game == nullptr || !this->_game->isRunning())
		return;
	else
		this->_window.render();
}

// Interpret user keyboard and GUI input as Commands and push
// commands to the Controller
void GraphicsView::pollInput(void)
{
	if (this->_controller == nullptr)
		return;

	// process Qt events
	this->_app.processEvents();

	// process window button input
	std::vector<std::string> buttonInput = this->_window.getButtonInput();
	for (auto& input : buttonInput) {
		// if the button input is a valid command, send it to the controller
		if (buttonCommandMap.find(input) != buttonCommandMap.end())
			this->_controller->push(Command(buttonCommandMap.at(input)));
	}

	// process keyboard input
	std::vector<int> keyboardInput = this->_window.getKeyboardInput();
	for (auto& input : keyboardInput) {
		// if the keyboard input is a valid command, send it to the controller
		if (keyCommandMap.find(input) != keyCommandMap.end())
			this->_controller->push(Command(keyCommandMap.at(input)));
	}

	// user closed graphics view
	if (!this->_window.isOpen())
		this->_shutdown();
}

// Unsubscribe from game here so that parent view cannot be subject to an
// attempted update. Set _open member to false so that if this view is
// in a ViewManager, the ViewManager will not poll this view for input
void GraphicsView::_shutdown(void)
{
	// unsubscribing stops game from notifying view
	if (this->_game)
		this->_game->unsubscribe(this);
	this->_subscribed = false;

	// closing stops view manager letting view respond to input
	this->_open = false;
}

bool GraphicsView::isOpen(void) const
{
	return this->_open;
}
