#ifndef GAME_H
#define GAME_H

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#include "../generic/Subject.h"
#include "../blocks/Board.h"
#include "../controller/Command.h"
#include "Level.h"
#include "StandardLevels.h"

class Level;

class Game: public Subject
{
public:
	Game(uint, std::string);
	~Game();
	void update(const Command& command);
	bool isRunning(void) const;

	const Board& getBoard(void) const;

	// gameplay methods
	void launch(void);
	void restart(void);
	void quit(void);

	int getNumBlocksSinceClear(void);
	unsigned int getScore(void) const;
	unsigned int getHighScore(void) const;
	int getLevel(void) const;

private:
	void _changeLevel(bool up);
	void _handleDrop(void);
	void _updateScore(int rowsCleared);
	void _setNextBlock(void);
	void _setBlockFromCache();
	bool _sequencedMode();

	Board _board;
	std::string _startSequence;
	std::unique_ptr<Level> _level;
	std::unordered_map<int, std::shared_ptr<Block>> _prevLevelBlocks;
	std::shared_ptr<Block> _sequenceCache;
	bool _running;
	unsigned int _score;
	unsigned int _highScore;
	unsigned int _startLevel;
};

#endif // GAME_H
