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

// Default block types
enum class BlockType : char {
	I = 'I',
	J = 'J',
	L = 'L',
	O = 'O',
	S = 'S',
	Z = 'Z',
	T = 'T',
	D = '*', // D block is displayed as '*' in console views
	HINT = '?',
	EMPTY = '.'
};

// Blocks are comprised of multiple cells
class Cell
{
public:
	Cell(int x, int y, std::string png, bool isDeleted, BlockType type);

	std::string getSprite(void) const;
	Coord getCoord(void) const;
	int get_x(void) const;
	int get_y(void) const;
	char getToken(void) const;
	BlockType getType(void) const;
	bool getCleared(void) const;
	bool isEmpty(void) const;

private:
	friend class Game;
	friend class Board;
	friend class Block;

	Coord _coords;
	BlockType _type;
	std::string _sprite;
	bool _cleared;
};

class Block
{
public:
	virtual ~Block() {};

	BlockType getType(void);
	std::vector<std::shared_ptr<Cell>>& getCells(void);
	bool isHeavy(void) const;
	bool isCleared(void) const;
	void blockSpace(bool blockSpace);
	void reflectInYeqX(void);
	void flipX(void);
	int yMax(void);
	int getLevelGenerated(void) const;
	void setSprite(const std::string& sprite);
	void setType(BlockType type);
	void copyCells(std::shared_ptr<Block> block);

protected:
	friend class Game;
	friend class Board;

	BlockType _type;
	Coord _bottomLeft;
	std::vector<std::shared_ptr<Cell>> _cells;

	int _levelGenerated;
	bool _isHeavy;
	bool _inBlockSpace;
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
