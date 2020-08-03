#include "Block.h"
#include "StandardBlocks.h"

// Coord Operator Implementations ==============================================
Coord operator+(Coord& a, Coord& b)
{
	return Coord(a._x + b._x, a._y + b._y);
}

Coord& Coord::operator+=(Coord &b)
{
	this->_x += b._x;
	this->_y += b._y;
	return *this;
}

Coord operator-(Coord& a, Coord& b)
{
	return Coord(a._x - b._x, a._y - b._y);
}

Coord Coord::invert(void)
{
	return Coord(this->_y, this->_x);
}

// Cell Constructor ============================================================
Cell::Cell(int x, int y, std::string png, bool isCleared, BlockType type)
	: _coords{x, y}, _type{type}, _sprite{png}, _cleared{isCleared}
{

}


// Cell Getter Methods =========================================================
std::string Cell::getSprite(void) const
{
	return this->_sprite;
}

Coord Cell::getCoord(void) const
{
	return this->_coords;
}

int Cell::get_x(void) const
{
	return this->_coords._x;
}

int Cell::get_y(void) const
{
	return this->_coords._y;
}

char Cell::getToken(void) const
{
	return (char)this->_type;
}

BlockType Cell::getType(void) const
{
	return this->_type;
}

bool Cell::isEmpty(void) const
{
	return this->_type == BlockType::EMPTY;
}

bool Cell::getCleared(void) const
{
	return this->_cleared;
}

// Define static instances for BlockFactory and Initializer with default blocks
std::map<char, std::unique_ptr<BlockFactory>> BlockFactory::_factories;
BlockFactoryInitializer BlockFactoryInitializer::bfi;

BlockFactoryInitializer::BlockFactoryInitializer()
{
	BlockFactory::_factories['I'] = std::make_unique<IBlock::Factory>();
	BlockFactory::_factories['J'] = std::make_unique<JBlock::Factory>();
	BlockFactory::_factories['L'] = std::make_unique<LBlock::Factory>();
	BlockFactory::_factories['O'] = std::make_unique<OBlock::Factory>();
	BlockFactory::_factories['S'] = std::make_unique<SBlock::Factory>();
	BlockFactory::_factories['Z'] = std::make_unique<ZBlock::Factory>();
	BlockFactory::_factories['T'] = std::make_unique<TBlock::Factory>();
	BlockFactory::_factories['D'] = std::make_unique<DBlock::Factory>();
}

std::shared_ptr<Block> BlockFactory::createBlock(const char& id, int level)
{
	if( _factories.find(id) != _factories.end() )
		return _factories[id]->create(level);

	return nullptr;
}

// Block Constructors =========================================================
IBlock::IBlock(int level)
{
	this->_type = BlockType::I;
	this->_bottomLeft = Coord(0,14);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,14,"./assets/t.png",
	                          false,BlockType::I));
	this->_cells.emplace_back(std::make_shared<Cell>(1,14,"./assets/t.png",
	                          false,BlockType::I));
	this->_cells.emplace_back(std::make_shared<Cell>(2,14,"./assets/t.png",
	                          false,BlockType::I));
	this->_cells.emplace_back(std::make_shared<Cell>(3,14,"./assets/t.png",
	                          false,BlockType::I));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

JBlock::JBlock(int level)
{
	this->_type = BlockType::J;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,14,"./assets/n.png",
	                          false,BlockType::J));
	this->_cells.emplace_back(std::make_shared<Cell>(0,13,"./assets/n.png",
	                          false,BlockType::J));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/n.png",
	                          false,BlockType::J));
	this->_cells.emplace_back(std::make_shared<Cell>(2,13,"./assets/n.png",
	                          false,BlockType::J));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

LBlock::LBlock(int level)
{
	this->_type = BlockType::L;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,13,"./assets/m.png",
	                          false,BlockType::L));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/m.png",
	                          false,BlockType::L));
	this->_cells.emplace_back(std::make_shared<Cell>(2,13,"./assets/m.png",
	                          false,BlockType::L));
	this->_cells.emplace_back(std::make_shared<Cell>(2,14,"./assets/m.png",
	                          false,BlockType::L));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

OBlock::OBlock(int level)
{
	this->_type = BlockType::O;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,13,"./assets/y.png",
	                          false,BlockType::O));
	this->_cells.emplace_back(std::make_shared<Cell>(0,14,"./assets/y.png",
	                          false,BlockType::O));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/y.png",
	                          false,BlockType::O));
	this->_cells.emplace_back(std::make_shared<Cell>(1,14,"./assets/y.png",
	                          false,BlockType::O));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

SBlock::SBlock(int level)
{
	this->_type = BlockType::S;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,13,"./assets/g.png",
	                          false,BlockType::S));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/g.png",
	                          false,BlockType::S));
	this->_cells.emplace_back(std::make_shared<Cell>(1,14,"./assets/g.png",
	                          false,BlockType::S));
	this->_cells.emplace_back(std::make_shared<Cell>(2,14,"./assets/g.png",
	                          false,BlockType::S));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

ZBlock::ZBlock(int level)
{
	this->_type = BlockType::Z;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,14,"./assets/r.png",
	                          false,BlockType::Z));
	this->_cells.emplace_back(std::make_shared<Cell>(1,14,"./assets/r.png",
	                          false,BlockType::Z));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/r.png",
	                          false,BlockType::Z));
	this->_cells.emplace_back(std::make_shared<Cell>(2,13,"./assets/r.png",
	                          false,BlockType::Z));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

TBlock::TBlock(int level)
{
	this->_type = BlockType::T;
	this->_bottomLeft = Coord(0,13);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(0,14,"./assets/p.png",
	                          false,BlockType::T));
	this->_cells.emplace_back(std::make_shared<Cell>(1,14,"./assets/p.png",
	                          false,BlockType::T));
	this->_cells.emplace_back(std::make_shared<Cell>(1,13,"./assets/p.png",
	                          false,BlockType::T));
	this->_cells.emplace_back(std::make_shared<Cell>(2,14,"./assets/p.png",
	                          false,BlockType::T));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}

DBlock::DBlock(int level)
{
	this->_type = BlockType::D;
	this->_bottomLeft = Coord(5,17);
	this->_inBlockSpace = false;

	this->_cells.emplace_back(std::make_shared<Cell>(5,17,"./assets/b.png",
	                          false,BlockType::D));

	this->_levelGenerated = level;
	this->_isHeavy = (level >= 3);
}


// Block Methods ===============================================================
BlockType Block::getType(void)
{
	return this->_type;
}

std::vector<std::shared_ptr<Cell>>& Block::getCells(void)
{
	return this->_cells;
}

bool Block::isHeavy(void) const
{
	return this->_isHeavy;
}

bool Block::isCleared(void) const
{
	for (auto c: this->_cells) {
		if (!c->getCleared())
			return false;
	}
	return true;
}

int Block::getLevelGenerated(void) const
{
	return this->_levelGenerated;
}

// Convert a block between block space and board space
// In block space, the origin for cell coordinates is the blocks bottom left
// In board space, the block's cell coordinates are relative to a Board
void Block::blockSpace(bool blockSpace)
{
	// if a block is already in block space or already in board space,
	// do not modify cell coordinates
	if (blockSpace == this->_inBlockSpace)
		return;

	for (auto& cell : this->_cells) {
		if (blockSpace)
			cell->_coords = cell->_coords - this->_bottomLeft;
		else
			cell->_coords = cell->_coords + this->_bottomLeft;
	}
	this->_inBlockSpace = blockSpace;
}

// Returns the largest y-coordinate of its cells
int Block::yMax(void)
{
	int max = 0;
	for (auto& cell : this->_cells)
		max = std::max(max, cell->get_y());
	return max;
}

// Reflects the cell coordinates in y = x
void Block::reflectInYeqX(void)
{
	for (auto& cell : this->_cells)
		cell->_coords = cell->_coords.invert();
}

// Reflect the block along the horizontal axis
// x = median x value of block cell coordinates
void Block::flipX(void)
{
	int maxY = this->yMax();
	for (auto& cell : this->_cells)
		cell->_coords._y = maxY - cell->get_y();
}

// Sets the sprite of all of the block's cells
void Block::setSprite(const std::string& sprite)
{
	for (auto& c : this->_cells)
		c->_sprite = sprite;
}

// Sets the type of a block (and updates its cells)
void Block::setType(BlockType type)
{
	this->_type = type;
	for (auto& c : this->_cells)
		c->_type = type;
}

// Copys the passed blocks cells
void Block::copyCells(std::shared_ptr<Block> block)
{
	this->_cells.clear();
	for (auto& c : block->getCells()) {
		Cell cell = *c;
		this->_cells.push_back(std::make_shared<Cell>(cell));
	}
}
