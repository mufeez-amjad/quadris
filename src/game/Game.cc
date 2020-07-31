#include "Game.h"

#include <iostream>
#include <string>

#include "./blocks/StandardBlocks.h"
Game::Game(uint startLevel, std::string filePath)
	: _board("./assets/_.png"), _defaultSequence{ filePath }, _running{true},
	_score{0}, _highScore{0}, _startLevel{startLevel}
{
	std::shared_ptr<std::ifstream> sequenceFile = std::make_shared<std::ifstream>();
	this->_level = LevelFactory::createLevel(startLevel, filePath, this, true, sequenceFile);
}

Game::~Game()
{

}

const Board& Game::getBoard(void) const
{
	return this->_board;
}

void Game::update(const Command& command)
{
	bool dropped = false;

	switch(command.type) {
		case CMD::QUIT:
			this->_running = false;
			break;

		case CMD::DOWN:
			this->_board.translate(Direction::DOWN);
			break;

		case CMD::LEFT:
			this->_board.translate(Direction::LEFT);
			break;

		case CMD::RIGHT:
			this->_board.translate(Direction::RIGHT);
			break;

		case CMD::DROP:
			dropped = this->_board.drop();
			break;

		case CMD::RESTART:
			this->restart();
			break;

		case CMD::LEVELUP:
			this->levelup();
			break;

		case CMD::LEVELDOWN:
			this->leveldown();
			break;

		// random block generation (sequence file remains unchanged)
		case CMD::RANDOM:
			this->_level->useRandom(true);
			break;

		// non-random block generation from the specified sequence file
		case CMD::NORANDOM_FILE:
			// ignore when level number is less than 3
			if (this->_level->getLevel() < 3)
				break;
			this->_level->openSequence(command.message);
			this->_level->useRandom(false);
			break;

		// run a sequence of commands from a file (for testing)
		case CMD::SEQUENCE_FILE:
			// set a new sequence file
			break;

		default:
			break;
	}

	if (dropped) {
		++this->_board._numBlockSinceClear;
		// put next block on the board
		this->_board.insertCurrentBlock();
		auto nextBlock = this->_level->getNextBlock();
		if (this->_board.setCurrentBlock(nextBlock) == false) {
			// if a new block cannot be added, the game is over
			this->_updateScore();
			this->restart();
			std::cerr << "Game Over!\n";
		}

		if(this->_board._currentBlock->getType() == BlockType::D) {
			this->_board.drop();
			++this->_board._numBlockSinceClear;
			// put next block on the board
			this->_board.insertCurrentBlock();
			auto nextBlock = this->_level->getNextBlock();
			if (this->_board.setCurrentBlock(nextBlock) == false) {
				// if a new block cannot be added, the game is over
				this->_updateScore();
				this->restart();
				std::cerr << "Game Over!\n";
			}
		}
	}

	if (!command.silent)
		this->_notify();
}

void Game::launch(void)
{
	auto firstBlock = this->_level->getNextBlock();
	if (this->_board.setCurrentBlock(firstBlock) == false) {
		std::cerr << "could not add first block\n";
	}
	this->_notify();
}

void Game::restart(void)
{
	// reset the board
	this->_board.reset();

	// reset the level
	this->_level->closeSequence();
	this->_level = LevelFactory::createLevel(this->_startLevel,
	                                         this->_defaultSequence,
	                                         this,
	                                         true,
	                                         this->_level->_sequence);

	auto nextBlock = this->_level->getNextBlock();
	if (this->_board.setCurrentBlock(nextBlock) == false) {
		std::cerr << "could not add next block\n";
	}
}

bool Game::isRunning(void) const
{
	return this->_running;
}

void Game::_updateScore(void)
{
	// TODO: implement
}

int Game::getNumBlocksSinceClear(void)
{
	return this->_board._numBlockSinceClear;
}

void Game::levelup(void)
{
	if (this->_level->getLevel() < 4) {
		int newLevel = this->_level->_level + 1;
		this->_level = LevelFactory::createLevel(newLevel,
		                                         _level->_filePath,
		                                         this,
		                                         _level->_random,
		                                         _level->_sequence);
	}
	// std::cerr << "Level UP to : " << this->_level->_level << "\n";
}

void Game::leveldown(void)
{
	if (this->_level->getLevel() > 0) {
		int newLevel = this->_level->_level - 1;
		this->_level = LevelFactory::createLevel(newLevel,
		                                         _level->_filePath,
		                                         this,
		                                         _level->_random,
		                                         _level->_sequence);
	}
	// std::cerr << "Level D./OWN to : " << this->_level->_level << "\n";
}
