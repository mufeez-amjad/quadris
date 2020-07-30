#ifndef CELL_H
#define CELL_H

#include "../graphics/Sprite2D.h"
#include "Block.h"

#include <string>

struct Coord
{
	int _x;
	int _y;
	
	Coord() {};
    Coord(uint x, uint y) : _x(x), _y(y) {};
};

class Cell
{
public:
	Cell(int x, int y, std::shared_ptr<Block> parent, std::string png, bool isDeleted);

	Sprite2D getSprite(void);
	int get_x(void);
	int get_y(void);
private:

	Coord _coords;
	std::shared_ptr<Block> _parent;
	Sprite2D _sprite;
	bool _isDeleted;
};

#endif // CELL_H
