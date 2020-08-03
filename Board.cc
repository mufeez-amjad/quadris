#include "Board.h"

#include <cassert>
#include <iostream>

Board::Board()
	: _cellSize{ default_cell_size }, _numBlockSinceClear{ 0 }
{

}

Board::Board(std::string png, float cellSize)
	: _cellSize{ cellSize }, _emptyCellSprite{ png },
	_currentBlock{ nullptr }, _nextBlock{ nullptr }, _hintBlock{ nullptr },
	_numBlockSinceClear{ 0 }
{
	this->reset();
}

void Board::reset(void)
{
	// clear the board
	for (auto& r : this->_board)
		r.clear();
	this->_board.clear();
	// clear blocks
	this->_blocks.clear();
	// clear the current block
	this->_currentBlock = nullptr;
	// clear the next block
	this->_nextBlock = nullptr;
	// initialize the board to empty cells
	for (int j = 0; j < BOARD_WIDTH; ++j) {
		std::vector<std::shared_ptr<Cell>> row;
		for(int i = 0; i < BOARD_HEIGHT; ++i){
			auto newCell = std::make_shared<Cell>(j, i, this->_emptyCellSprite,
			                                      false, BlockType::EMPTY);
			row.push_back(newCell);
		}
		this->_board.push_back(row);
	}
	// reset the number of blocks since last clear count
	this->_numBlockSinceClear = 0;
}

Cell Board::at(Coord coord) const
{
	assert(   0 <= coord._x && coord._x < BOARD_WIDTH
	       && 0 <= coord._y && coord._y < BOARD_HEIGHT);

	return *this->_board[coord._x][coord._y];
}

block_ptr Board::getCurrentBlock(void) const
{
	return this->_currentBlock;
}

void Board::setNextBlock(block_ptr nextBlock)
{
	if (nextBlock == nullptr)
		return;
	this->_nextBlock = nextBlock;
}

block_ptr Board::getNextBlock(void) const
{
	return this->_nextBlock;
}

block_ptr Board::getHintBlock(void) const
{
	return this->_hintBlock;
}

std::vector<block_ptr>& Board::getBlocks(void)
{
	return this->_blocks;
}

float Board::getCellSize(void) const
{
	return this->_cellSize;
}

BoardIterator Board::begin() const
{
	return BoardIterator(0, this->_board[0].size()-1, this->_board[0].size(),
	                     this->_board.size(), this->_board);
}

BoardIterator Board::end() const
{
	return BoardIterator(this->_board.size(), -1, this->_board.size(),
	                     this->_board[0].size(), this->_board);
}


bool Board::_inBounds(int x, int y)
{
	return 0 <= x && x < BOARD_WIDTH && 0 <= y && y < BOARD_HEIGHT;
}

bool Board::_inBounds(Coord coord)
{
	return this->_inBounds(coord._x, coord._y);
}

Coord Board::_directionDeltas(Direction direction)
{
	int dx = 0;
	int dy = 0;
	switch (direction) {
		case Direction::UP:
			dy = 1;
			break;
		case Direction::DOWN:
			dy = -1;
			break;
		case Direction::LEFT:
			dx = -1;
			break;
		case Direction::RIGHT:
			dx = 1;
			break;
		case Direction::NONE:
		default:
			break;
	}
	return Coord(dx, dy);
}

bool Board::_validTranslation(Direction direction, block_ptr block)
{
	Coord delta = this->_directionDeltas(direction);
	auto cells = block->getCells();
	for (auto& c : cells) {
		Coord newPos = c->_coords + delta;
		// if the new position is out of bounds, return false
		if (!this->_inBounds(newPos))
			return false;

		// if the new position is not empty cells, return false
		if (this->at(newPos).getType() != BlockType::EMPTY)
			return false;
	}
	return true;
}

void Board::_doTranslation(Direction direction, block_ptr block)
{
	Coord delta = this->_directionDeltas(direction);
	block->_bottomLeft += delta;
	for (auto& c : block->getCells())
		c->_coords += delta;
}

// insert the given block's cells into the board
// this method does not check if the board is currently occupied
// at the blocks position
void Board::_insertBlock(block_ptr block)
{
	for (auto& c : block->getCells())
		this->_board[c->get_x()][c->get_y()] = c;
}

// translate the currently active block in the given direction
// translates by 1 cell in the direction specified
bool Board::translate(Direction direction, block_ptr block)
{
	if (this->_validTranslation(direction, block)) {
		this->_doTranslation(direction, block);
		return true;
	}
	return false;
}

bool Board::translate(Direction direction)
{
	return this->translate(direction, this->_currentBlock);
}


// translate the current block down until it is no longer a valid translation
void Board::drop(block_ptr block)
{
	while (this->_validTranslation(Direction::DOWN, block))
		this->_doTranslation(Direction::DOWN, block);
}

void Board::drop(void)
{
	this->drop(this->_currentBlock);
}

// rotate a block
bool Board::rotate(bool clockwise, block_ptr block)
{
	// rotate in the given direction
	this->_doRotation(clockwise, block);

	// check if the resulting position is valid
	if (this->_validTranslation(Direction::NONE, block)) {
		// rotation was valid
		return true;
	} else {
		// rotation was invalid, so undo rotation
		this->_doRotation(!clockwise, block);
		return false;
	}
}

bool Board::rotate(bool clockwise)
{
	return this->rotate(clockwise, this->_currentBlock);
}

void Board::_doRotation(bool clockwise, block_ptr block)
{
	// convert the current block coordinates to block space
	block->blockSpace(true);
	// perform rotation
	if (clockwise) {
		block->reflectInYeqX();
		block->flipX();
	} else {
		block->flipX();
		block->reflectInYeqX();
	}
	// convert current block back to board space
	block->blockSpace(false);
}

void Board::_shiftSingleRowDown(int row, int offset)
{
	// offset must be in the board
	if (row - offset < 0)
		return;

	for (int x = 0; x < BOARD_WIDTH; x++) {
		// shift cell down
		this->_board[x][row - offset] = this->_board[x][row];
		// update y coordinate
		this->_board[x][row - offset]->_coords._y = row - offset;
		// make current cell empty
		this->_clearCell(x, row);
	}
}

void Board::_shiftDown(int bottom, int highest, int offset)
{
	// 1. Ensure that the number of empty consecutive rows below the first row
	//    to shift down is equal to offset. The first to shift down is always
	//    the row that is 4 cells above the bottom left corner of the block
	//    that completed row(s)
	for (int i = bottom + 1; i <= highest; i++) {
		// if the row below is empty, shift this row down
		if (this->_isRowEmpty(i - 1))
			this->_shiftSingleRowDown(i);
	}

	// 2. Shift every row above the highest empty row down by offset number of
	//    blocks.
	for (int i = highest + 1; i < (BOARD_HEIGHT - 3); i++)
		this->_shiftSingleRowDown(i, offset);
}

// Sets a cell in the board to be empty
void Board::_clearCell(int x, int y)
{
	auto c = std::make_shared<Cell>(x,
	                                y,
	                                this->_emptyCellSprite,
	                                false,
	                                BlockType::EMPTY);
	this->_board[x][y] = c;
}

// Set all the cells in a row to empty
void Board::_clearSingleRow(int row)
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		// set Block's cells to cleared
		_board[x][row]->_cleared = true;
		// reset row to contain empty cells
		this->_clearCell(x, row);
	}
}

// return true if a row is empty else false
bool Board::_isRowEmpty(int row)
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		if (!this->_board[x][row]->isEmpty())
			return false;
	}
	return true;
}

// return true if a row is full
bool Board::_isRowFull(int row)
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		if (this->_board[x][row]->isEmpty())
			return false;
	}
	return true;
}

// remove filled rows and shift down cells above the cleared rows
// return the number of rows cleared
int Board::_clearRows(void)
{
	int bottom = this->_currentBlock->_bottomLeft._y;
	int top = this->_currentBlock->yMax();
	int rowsCleared = 0;
	int highestCleared = 0;

	for (int y = bottom; y <= top; y++) {
		if (!this->_isRowFull(y))
			continue;

		// row needs to be cleared
		highestCleared = y;
		rowsCleared++;
		this->_clearSingleRow(y);
	}

	if (rowsCleared != 0)
		this->_shiftDown(bottom, highestCleared, rowsCleared);
	return rowsCleared;
}

// insert the cells of the currently active block into the board
int Board::insertCurrentBlock(void)
{
	if (this->_validTranslation(Direction::NONE, this->_currentBlock)) {
		// insert the current block's cells into the board
		this->_insertBlock(this->_currentBlock);
		// put the current block into the vector of blocks,
		// since a new block should become current
		this->_blocks.push_back(this->_currentBlock);
		return this->_clearRows();
	}
	return -1;
}

bool Board::setCurrentBlock(block_ptr currentBlock)
{
	this->_currentBlock = currentBlock;

	// on recieving a nullptr, a level sequence file has reached
	// EOF, and the game is over
	if (currentBlock == nullptr)
		return false;

	if (this->_validTranslation(Direction::NONE, this->_currentBlock))
		return true;
	this->_currentBlock = nullptr;
	return false;
}

bool Board::hasHint(void) const
{
	return this->_hintBlock != nullptr;
}

void Board::removeHint(void)
{
	this->_hintBlock = nullptr;
}

void Board::createHintBlock(void)
{
	// assert(this->_currentBlock != nullptr);

	// // create hint block copy of current block
	// Block hintBlock = *this->_currentBlock;
	// hintBlock.copyCells(this->_currentBlock);
	// hintBlock.setType(BlockType::HINT);
	// hintBlock.setSprite("./assets/b_.png"); // TODO: need black sprite
	// this->_hintBlock = std::make_shared<Block>(hintBlock);
}
