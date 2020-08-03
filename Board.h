#ifndef BOARD_H
#define BOARD_H

#include "BoardIterator.h"
#include "Block.h"

#include <vector>

#define BOARD_HEIGHT 18
#define BOARD_WIDTH 11

float constexpr default_cell_size = 25.0f;

typedef std::shared_ptr<Block> block_ptr;

class Window;
class ConsoleView;

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NONE
};

class Board
{
public:
	Board();
	Board(std::string png, float cellSize = default_cell_size);

	Cell at(Coord coord) const;

	float getCellSize(void) const;

	// clears all cells and the block vector
	void reset(void);

	// translates a block in the given direction
	bool translate(Direction direction, block_ptr block);
	bool translate(Direction direction);

	// drops a block
	void drop(block_ptr block);
	void drop(void);

	// insert the currently active block cells into the table
	int insertCurrentBlock(void);

	// rotates a block
	bool rotate(bool clockwise, block_ptr block);
	bool rotate(bool clockwise);

	// show hint
	void createHintBlock(void);
	// true if hint is being displayed
	bool hasHint(void) const;
	// remove the hint block
	void removeHint(void);

	// set the currently active block
	bool setCurrentBlock(block_ptr currentBlock);

	// get the currently active block
	block_ptr getCurrentBlock(void) const;

	// set the next block
	void setNextBlock(block_ptr nextBlock);

	// get the next block
	block_ptr getNextBlock(void) const;

	// get the hint block
	block_ptr getHintBlock(void) const;

	std::vector<block_ptr>& getBlocks(void);

	BoardIterator begin(void) const;
	BoardIterator end(void) const;

private:
	friend class Game;

	float _cellSize;

	void _insertBlock(block_ptr block);
	int _clearRows(void);

	// row filled helpers
	void _clearSingleRow(int row);
	void _shiftDown(int bottom, int highest, int offset);
	bool _isRowEmpty(int row);
	bool _isRowFull(int row);
	void _shiftSingleRowDown(int row, int offset = 1);
	void _clearCell(int x, int y);

	// board boundary checking
	bool _inBounds(int x, int y);
	bool _inBounds(Coord coord);

	// block translation helpers
	void _doTranslation(Direction direction, block_ptr block);
	bool _validTranslation(Direction direction, block_ptr block);
	Coord _directionDeltas(Direction direction);

	// block rotation helpers
	void _doRotation(bool clockwise, block_ptr block);

	std::string _emptyCellSprite;
	std::vector<std::vector<std::shared_ptr<Cell>>> _board;
	std::vector<block_ptr> _blocks;

	block_ptr _currentBlock;
	block_ptr _nextBlock;
	block_ptr _hintBlock;

	int _numBlockSinceClear;
};

#endif // BOARD_H
