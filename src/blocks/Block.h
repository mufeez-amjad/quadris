#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <memory>
#include <map>

// 2D Coordinate
struct Coord
{
	int _x;
	int _y;

	Coord() {};
	Coord(int x, int y) : _x{ x }, _y{ y } {}

	Coord& operator+=(Coord& b);
	Coord invert(void);
};

// Coordinate operators
Coord operator+(Coord& a, Coord& b);
Coord operator-(Coord& a, Coord& b);


enum class BlockType : char {
	I = 'I',
	J = 'J',
	L = 'L',
	O = 'O',
	S = 'S',
	Z = 'Z',
	T = 'T',
	D = '*',
	EMPTY = '.'
};

// Blocks are comprised of multiple cells
class Cell
{
public:
	Cell(int x, int y, std::string png, bool isDeleted, BlockType type);

	std::string getSprite(void);
	Coord getCoord(void) const;
	int get_x(void);
	int get_y(void);
	char getToken(void) const;
	BlockType getType(void) const;

	bool _isCleared;

private:
	friend class Board;

	Coord _coords;
	BlockType _type;
	std::string _sprite;
};

class Block
{
public:
	virtual ~Block() {};

	BlockType getType(void);
	std::vector<std::shared_ptr<Cell>>& getCells(void);

protected:
	friend class Board;

	BlockType _type;
	Coord _bottomLeft;
	std::vector<std::shared_ptr<Cell>> _cells;

	int _orientation;
	int _levelGenerated;
	bool _isHeavy;
};

class BlockFactoryInitializer
{
private:
	static BlockFactoryInitializer bfi;

	BlockFactoryInitializer();
	~BlockFactoryInitializer() {};
};


class BlockFactory
{
public:
	virtual ~BlockFactory() {};
	static std::shared_ptr<Block> createBlock(const char&, int);

private:
	friend class BlockFactoryInitializer;
	static std::map<char, std::unique_ptr<BlockFactory>> _factories;
	virtual std::shared_ptr<Block> create(int) = 0;
};

#endif // BLOCK_H
