#include "ConsoleView.h"

#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <unordered_set>

// generated with: http://patorjk.com/software/taag/#p=display&f=Small&t=Quadris
// modifications to source material:
//    - added escaped characters to escape '\'
//    - added '\n' characters
static const char* quadrisTitle =
"   ___               _     _    \n"
"  / _ \\ _  _ __ _ __| |_ _(_)___\n"
" | (_) | || / _` / _` | '_| (_-<\n"
"  \\__\\_\\\\_,_\\__,_\\__,_|_| |_/__/\n\n";

ConsoleView::ConsoleView(Game* game, Controller* controller, std::istream& in,
                         std::ostream& out)
	: View{ game, controller }, _usingScript{ false },
	_issuedQuitCmd{ false }, _consoleIn{ in }, _out{ out }
{
	// construct trie
	this->_trie = std::make_shared<Trie>();
	this->_buildTrie();

	this->_clearConsole();
}

void ConsoleView::launch(void)
{
	// spin thread to read _in stream
	this->_inThread = std::thread(&ConsoleView::readInStream, this);
}

ConsoleView::~ConsoleView()
{
	// close open file streams
	if (this->_usingScript && this->_scriptIn.is_open())
		this->_scriptIn.close();

	// clean up console otherwise input box artifacts remain
	this->_clearConsole();

	// join the _inThread before application exits
	if (this->_inThread.joinable())
		this->_inThread.join();
}

void ConsoleView::_readActiveInputStream(std::string& str)
{
	if (this->_usingScript)
		this->_scriptIn >> str;
	else
		this->_consoleIn >> str;
}

void ConsoleView::_buildTrie()
{
	std::ifstream infile("./commands.txt");
	std::string command;
	int id;
	while (infile >> command >> id)
		_trie->push(command, (CommandType) id);
}

std::vector<Command> ConsoleView::_processCommand(const std::string& s)
{
	std::unordered_set<CommandType> invalidMultipliers = { CMD::RANDOM,
	                                                       CMD::RESTART,
	                                                       CMD::HINT,
	                                                       CMD::NORANDOM_FILE,
	                                                       CMD::SEQUENCE_FILE };
	std::vector<Command> commands;
	unsigned int split = 0;
	for (; split < s.length(); split++) {
		char c = s[split];
		if (c > '9' || c < '0')
			break;
	}

	std::string multiplierString = s.substr(0, split);
	int multiplier = multiplierString.length() ?
	                 std::stoi(multiplierString) : 1;
	std::string command = s.substr(split);
	Command matchingCommand = _trie->findShortestPrefix(command);
	matchingCommand.silent = true;

	if (invalidMultipliers.find(matchingCommand.type)
	    != invalidMultipliers.end()) // force multipler of 1
		multiplier = 1;

	if (matchingCommand.type == CMD::QUIT)
		this->_issuedQuitCmd = true;

	if (matchingCommand.type != CMD::UNDEFINED_COMMAND) {
		for (int i = 0; i < multiplier; i++)
			commands.push_back(matchingCommand);
	}

	if (!commands.empty()) // set last command to not silent
		commands.back().silent = false;

	return commands;
}

bool ConsoleView::_isValidFilePath(const std::string& filePath) {
	std::ifstream infile(filePath);
	return infile.good();
}

// readInStream runs in separate thread for the lifetime
// of a ConsoleView instance. This method may attempt to
// call methods of Controller, and Subject::unsubscribe,
// so these classes / methods should ensure that data members are
// thread safe in instances where readInStream may call them
void ConsoleView::readInStream(void)
{
	std::string command;

	while (this->_game->isRunning()) {
		this->_readActiveInputStream(command);

		if (this->_usingScript && this->_scriptIn.eof()) {
			this->_scriptIn.close();
			this->_usingScript = false;
			continue;
		}

		// Since the game may terminate while waiting for input, check if the
		// game is still running before  sending the command
		if (this->_game != nullptr && this->_game->isRunning()) {
			std::vector<Command> payload = this->_processCommand(command);

			bool sendToController = !payload.empty();

			if (payload.size() == 1) {
				CommandType type = payload.at(0).type;
				// attach filePath to command
				if (type == CMD::NORANDOM_FILE || type == CMD::SEQUENCE_FILE) {
					std::string filePath;
					this->_readActiveInputStream(filePath);

					sendToController = this->_isValidFilePath(filePath);

					if (type == CMD::SEQUENCE_FILE) {
						this->setScript(filePath);
						sendToController = false; // don't need to tell controller
					}
					if (type == CMD::NORANDOM_FILE)
						payload.at(0).message = filePath;
				}
			}

			if (sendToController)
				this->_controller->push(payload);
			else
				this->update(); //requery

			// when quitting, kill thread immediately
			if (this->_issuedQuitCmd) {
				if (this->_game != nullptr) {
					// Here, this view closes itself in a separate thread, and
					// its base class may still exist when game begins notifying
					// observers it has recieved a "quit" command. To ensure
					// that game does not try to call notify on the virtual View
					// base class from which ConsoleView is described,
					// unsubscribe here to ensure complete destruction or
					// destruction after recieving Game's notification.
					this->_game->unsubscribe(this);
					this->_subscribed = false;
				}
				return;
			}
		}
	}
}

void ConsoleView::_clearConsole(void)
{
	// if using a script file, do not clear output stream
	// in order for script output to be piped into file
	if (this->_usingScript)
		return;

	if (system("clear") == 0 || system("cls") == 0)
		return;
}

void ConsoleView::_displayGame(const Board& board)
{
	std::string display; // next console frame
	std::vector<char> boardChars = this->_createBoardChars(board);
	std::vector<std::string> next = this->_createNextStrings(board);
	this->_prepareDisplay(display, boardChars, next);
	this->_clearConsole();
	this->_out << display;
	this->_out.flush();
}

std::vector<char> ConsoleView::_createBoardChars(const Board& board)
{
	// prepare board as a vector of their char representations
	std::vector<char> boardChars;
	for (auto i = board.begin(); i != board.end(); ++i)
		boardChars.push_back((*i).getToken());

	// overlay the active block's cells, if any
	auto currentBlock = board.getCurrentBlock();
	this->_overlaySpecialBlock(boardChars, currentBlock);

	// overlay the hint block's cells, if any
	auto hintBlock = board.getHintBlock();
	this->_overlaySpecialBlock(boardChars, hintBlock);

	return boardChars;
}

void ConsoleView::_overlaySpecialBlock(std::vector<char>& boardChars,
									   std::shared_ptr<Block> block)
{
	if (block != nullptr) {
		for (auto& c : block->getCells()) {
			int x = c->get_x();
			// top of board is flipped in display
			int y = 17 - c->get_y();
			unsigned int index = 11 * y + x;
			if (index < boardChars.size())
				boardChars.at(index) = c->getToken();
		}
	}
}

std::vector<std::string> ConsoleView::_createNextStrings(const Board& board)
{
	std::vector<std::string> next = { "        ", "        ", "        ",
									  "        " };
	std::shared_ptr<Block> nextBlock = board.getNextBlock();
	if (nextBlock == nullptr)
		return next;
	nextBlock->blockSpace(true);
	for (auto& cell : nextBlock->getCells())
		next.at(3 - cell->get_y()).at(2 * cell->get_x()) = cell->getToken();
	nextBlock->blockSpace(false);
	return next;
}

void ConsoleView::_prepareDisplay(std::string& display,
                                  std::vector<char>& boardChars,
								  std::vector<std::string>& next)
{
	display.append(quadrisTitle);
	int row = 1;
	int rowCount = 0;
	std::string board_string = "   1      ";
	for (auto& c : boardChars) {
		if (rowCount == 11) {
			// add text to the right of the quadris board (if applicable)
			this->_addInfo(row, board_string, next);
			row++;
			// add row number
			board_string.append("\n   " + std::to_string(row));
			// pad to board
			board_string.append(std::string(6 - (row / 10), ' '));
			rowCount = 1;
		} else {
			rowCount++;
		}

		board_string.append(std::string(1, c)); // add cell token
		board_string.append(" "); // add space between cells
	}
	display.append(board_string);
	if (!this->_usingScript) {
		display.append(
			"\n┌────────────────────────────────────────────────────────┐\n");
		display.append(
			  "│ >                                                      │\n");
		display.append(
			  "└────────────────────────────────────────────────────────┘");
		display.append("\x1b[A");
		display.append("\r│ > ");
	} else {
		display.append("\n\n");
	}
}

void ConsoleView::_addInfo(int row, std::string& display,
						   std::vector<std::string>& next)
{
	std::string info;
	int level;
	int score;
	int highScore;
	if ((2 <= row && row <= 4) || (8 <= row && row <= 12))
		info.append("    │ ");

	switch (row) {
	case 2:
		level = this->_game->getLevel();
		info.append("level: " + std::to_string(level));
		break;
	case 3:
		score = this->_game->getScore();
		info.append("score: " + std::to_string(score));
		break;
	case 4:
		highScore = this->_game->getHighScore();
		info.append("high score: " + std::to_string(highScore));
		break;
	case 1:
	case 7:
		info.append("    ┌────────────────────┐");
		break;
	case 5:
	case 13:
		info.append("    └────────────────────┘");
		break;
	case 8:
		info.append("next:");
		break;
	case 9:
	case 10:
	case 11:
	case 12:
		info.append(next.at(row - 9));
		break;
	default:
		return;
	}
	// right hand side of info text box
	if ((2 <= row && row <= 4) || (8 <= row && row <= 12)) {
		info.append(std::string(27 - info.length(), ' '));
		info.append("│");
	}
	display.append(info);
}

void ConsoleView::setScript(const std::string& scriptFile)
{
	if (!this->_isValidFilePath(scriptFile))
		return;

	this->_scriptFile = scriptFile;
	if (this->_scriptIn.is_open())
		this->_scriptIn.close();

	this->_scriptIn.open(this->_scriptFile);
	this->_usingScript = true;
	this->_out << "\n";
}

void ConsoleView::pollInput(void)
{
	// input stream must be polled from separate thread and not here
}

void ConsoleView::update(void)
{
	this->_displayGame(this->_game->getBoard());
}

// A normal ConsoleView tied to the program's execution and thus is
// open as long as the instance exists
bool ConsoleView::isOpen(void) const
{
	return true;
}
