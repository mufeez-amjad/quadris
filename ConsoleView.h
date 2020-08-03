#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include <iostream>
#include <thread>

#include "View.h"
#include "Game.h"
#include "Trie.h"

class ConsoleView : public View
{
public:
	ConsoleView(Game* game, Controller* controller, std::istream& in,
	            std::ostream& out);
	~ConsoleView();

	void pollInput(void) override;
	void readInStream(void);
	bool isOpen(void) const override;
	void setScript(const std::string& scriptFile);
	void launch(void) override;

	void update(void) override;

private:
	// Display methods
	void _displayGame(const Board& board);
	std::vector<char> _createBoardChars(const Board& board);
	void _overlaySpecialBlock(std::vector<char>& boardChars,
	                          std::shared_ptr<Block> block);
	std::vector<std::string> _createNextStrings(const Board& board);
	void _clearConsole(void);
	void _prepareDisplay(std::string& display,
	                     std::vector<char>& boardChars,
	                     std::vector<std::string>& next);
	void _addInfo(int row,
	              std::string& display,
	              std::vector<std::string>& next);

	// Trie methods
	void _buildTrie(void);
	std::vector<Command> _processCommand(const std::string&);

	// Read from the active input stream
	void _readActiveInputStream(std::string& str);

	// Command input script file
	std::string _scriptFile;
	bool _usingScript;
	std::ifstream _scriptIn;

	bool _isValidFilePath(const std::string& filePath);

	bool _issuedQuitCmd;
	std::shared_ptr<Trie> _trie;
	std::istream& _consoleIn;
	std::ostream& _out;
	std::thread _inThread;
};

#endif // CONSOLEVIEW_H
