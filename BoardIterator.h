#ifndef BOARDITERATOR_H
#define BOARDITERATOR_H

#include <vector>
#include <memory>

#include "Block.h"

class Board;

class BoardIterator
{
	friend class Board;

public:

	BoardIterator(int x, int y, int row, int col,
				  std::vector<std::vector<std::shared_ptr<Cell>>> board) :
		_y(y), _x(x), _rows(row), _cols(col), _cells(board) {};

	Cell operator*() const;
	BoardIterator& operator++();
	bool operator!=(const BoardIterator& other) const;

private:
	int _y;     // current row index of iterator in board
	int _x;     // current column index of iterator in board
	int _rows;  // number of rows in board
	int _cols;  // number of columns in board
	std::vector<std::vector<std::shared_ptr<Cell>>> _cells;
};

#endif // BOARDITERATOR_H
