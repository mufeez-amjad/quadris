#include "BoardIterator.h"

Cell BoardIterator::operator*() const
{
	return *this->_cells[this->_x][this->_y];
}

BoardIterator& BoardIterator::operator++()
{
	this->_x++;
	if (this->_x >= this->_cols) {
		this->_y--;
		if (this->_y >= 0)
			this->_x = 0;
	}

	return *this;
}

bool BoardIterator::operator!=(const BoardIterator& other) const
{
	return this->_cells != other._cells || this->_x != other._x
	                                    || this->_y != other._y;
}
