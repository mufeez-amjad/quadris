#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <fstream>

#include "Game.h"

class Game;

class Level
{
public:
	virtual ~Level() {};
	// Strategy pattern method for nextBlock
	virtual std::shared_ptr<Block> getNextBlock(int) = 0;
	// Basic method that forwards a call to the BlockFactory to generate
	// a specific block for IJLOSZT commands
	std::shared_ptr<Block> getBlock(const char&, int);

	int getLevel(void) const;

	// Methods to close and open the ifstream object for input from a
	// sequence.txt file
	void closeSequence(void);
	void openSequence(const std::string& sequenceFile);

	// Set random mode for levels
	virtual void useRandom(bool random);

protected:
	friend class Game;

	int _level;
	bool _random;
	std::string _filePath;
	std::shared_ptr<std::ifstream> _sequence;
	Game* _game;
};


class LevelFactoryInitializer
{
private:
	static LevelFactoryInitializer lfi;

	LevelFactoryInitializer();
	~LevelFactoryInitializer() {};
};


class LevelFactory
{
public:
	virtual ~LevelFactory() {};
	static std::unique_ptr<Level> createLevel(const int&,
											  std::string,
											  Game*,
	                                          bool,
											  std::shared_ptr<std::ifstream>);

private:
	friend class LevelFactoryInitializer;
	static std::map<int, std::unique_ptr<LevelFactory>> _factories;
	virtual std::unique_ptr<Level> create(std::string,
										  Game*,
										  bool,
										  std::shared_ptr<std::ifstream>) = 0;
};

#endif // LEVEL_H
