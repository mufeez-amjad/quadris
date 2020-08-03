#include <assert.h>
#include <iostream>
#include <map>

#include "StandardLevels.h"

// define static instances
std::map<int, std::unique_ptr<LevelFactory>> LevelFactory::_factories;
LevelFactoryInitializer LevelFactoryInitializer::lfi;

// base class level methods
int Level::getLevel(void) const
{
	return this->_level;
}

// Close ifstreasm object
void Level::closeSequence(void)
{
	if (this->_sequence->is_open())
		this->_sequence->close();
}

// Set new filePath and open it to ifstream object
void Level::openSequence(const std::string& sequenceFile)
{
	// if a sequence file is open, close it first
	this->closeSequence();
	this->_filePath = sequenceFile;
	this->_sequence->open(this->_filePath);
}

// Switch to random or norandom mode for sequenced input
void Level::useRandom(bool random)
{
	this->_random = random;
	// if switching to using sequence file and it is not open, open it
	if (!random && !this->_sequence->is_open())
		this->_sequence->open(this->_filePath);
}

std::shared_ptr<Block> Level::getBlock(const char& blockType, int level)
{
	return BlockFactory::createBlock(blockType, level);
}

LevelFactoryInitializer::LevelFactoryInitializer()
{
	LevelFactory::_factories[0] = std::make_unique<Level0::Factory>();
	LevelFactory::_factories[1] = std::make_unique<Level1::Factory>();
	LevelFactory::_factories[2] = std::make_unique<Level2::Factory>();
	LevelFactory::_factories[3] = std::make_unique<Level3::Factory>();
	LevelFactory::_factories[4] = std::make_unique<Level4::Factory>();
}

std::unique_ptr<Level> LevelFactory::createLevel(const int& 	lev,
												 std::string 	filePath,
												 Game* 			gameRef,
												 bool 			rand,
												 std::shared_ptr<std::ifstream>
												 				sin)
{
	if(_factories.find(lev) != _factories.end())
		return _factories[lev]->create(filePath, gameRef, rand, sin);

	return nullptr;
}

// Level0 Implementation
Level0::Level0(std::string filePath, Game* gameRef, bool rand,
			   std::shared_ptr<std::ifstream> sin)
{
	this->_level = 0;
	this->_random = rand;
	this->_filePath = filePath;
	this->_game = gameRef;
	this->_sequence = sin;
}

Level0::~Level0()
{

}

void Level0::useRandom(bool use)
{
	// Level0 always uses a sequence
}

std::shared_ptr<Block> Level0::getNextBlock(int level)
{
	char blockType = ' ';
	while (std::isspace(static_cast<unsigned char>(blockType)) != 0)
		blockType = this->_sequence->get();

	if (this->_sequence->eof())
		return nullptr;
	else
		return BlockFactory::createBlock(blockType, level);
}

// Returns random character from blocks with uniform probability
char randomBlock(std::string blocks)
{
	return blocks[std::rand() % blocks.size()];
}


// Level1 Implementation
Level1::Level1(std::string filePath, Game* gameRef, bool rand,
               std::shared_ptr<std::ifstream> sin)
{
	this->_level = 1;
	this->_random = rand;
	this->_filePath = filePath;
	this->_game = gameRef;
	this->_sequence = sin;
}

Level1::~Level1()
{

}

std::shared_ptr<Block> Level1::getNextBlock(int level)
{
	char nextBlockType = '.';
	switch (std::rand() % 6)
	{
		// case 0 with probability 1/6
		case 0:
			// randomBlock returns S or Z with uniform probability (1/2)
			// Pr('S') = Pr('Z') = Pr(case 0) * 1/2 = 1/6 * 1/2 = 1/12
			nextBlockType = randomBlock(std::string("SZ"));
			break;

		// default with probability 5/6
		default:
			// randomBlock returns one of IJLOT with uniform probability (1/5)
			// Pr(any of IJLOT) = Pr(default) * 1/5 = 5/6 * 1/5 = 1/6
			nextBlockType = randomBlock(std::string("IJLOT"));
			break;

	}
	return BlockFactory::createBlock(nextBlockType, level);
}


// Level2 Implementation
Level2::Level2(std::string filePath, Game* gameRef, bool rand,
               std::shared_ptr<std::ifstream> sin)
{
	this->_level = 2;
	this->_random = rand;
	this->_filePath = filePath;
	this->_game = gameRef;
	this->_sequence = sin;
}

Level2::~Level2()
{

}

std::shared_ptr<Block> Level2::getNextBlock(int level)
{
	// Any of IJLOSZT with uniform probability (1/7)
	char nextBlockType = randomBlock(std::string("IJLOSZT"));
	return BlockFactory::createBlock(nextBlockType, level);
}


// Level3 Implementation
Level3::Level3(std::string filePath, Game* gameRef, bool rand,
               std::shared_ptr<std::ifstream> sin)
{
	this->_level = 3;
	this->_random = rand;
	this->_filePath = filePath;
	this->_game = gameRef;
	this->_sequence = sin;
	this->_level0 = Level0(filePath, gameRef, rand, sin);
}

Level3::~Level3()
{

}

void Level3::useRandom(bool random)
{
	this->_random = random;
	// if switching to using sequence file and it is not open, open it
	if (!random && !this->_sequence->is_open())
		this->_sequence->open(this->_filePath);
}

std::shared_ptr<Block> Level3::getNextBlock(int level)
{
	if (!this->_random)
		return this->_level0.getNextBlock(level);

	char nextBlockType = '.';
	switch (std::rand() % 9)
	{
		case 0:
		case 1:
		case 2:
		case 3:
			nextBlockType = randomBlock(std::string("SZ"));
			break;
		default:
			nextBlockType = randomBlock(std::string("IJLOT"));
			break;
	}
	return BlockFactory::createBlock(nextBlockType, level);
}


// Level4 Implementation
Level4::Level4(std::string filePath, Game* gameRef, bool rand,
               std::shared_ptr<std::ifstream> sin)
{
	this->_level = 4;
	this->_random = rand;
	this->_filePath = filePath;
	this->_game = gameRef;
	this->_sequence = sin;
	this->_level3 = Level3(filePath, gameRef, rand, sin);
}

Level4::~Level4()
{

}

void Level4::useRandom(bool random)
{
	this->_random = random;
	this->_level3._random = random;
	// if switching to using sequence file and it is not open, open it
	if (!random && !this->_sequence->is_open())
		this->_sequence->open(this->_filePath);
}

std::shared_ptr<Block> Level4::getNextBlock(int level)
{
	if (!this->_random)
		return this->_level3._level0.getNextBlock(level);

	int bsc = this->_game->getNumBlocksSinceClear();
	if (bsc % 5 == 0 && bsc != 0)
		return BlockFactory::createBlock('D', level);

	return this->_level3.getNextBlock(level);
}
