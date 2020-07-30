#include "Block.h"
#include "StandardBlocks.h"

// define static instances
std::map<std::string, std::unique_ptr<BlockFactory>> BlockFactory::_factories;
BlockFactoryInitializer BlockFactoryInitializer::si;

BlockFactoryInitializer::BlockFactoryInitializer()
{
	BlockFactory::_factories["I"] = std::make_unique<IBlock::Factory>();
	BlockFactory::_factories["J"] = std::make_unique<JBlock::Factory>();
	BlockFactory::_factories["L"] = std::make_unique<LBlock::Factory>();
	BlockFactory::_factories["O"] = std::make_unique<OBlock::Factory>();
	BlockFactory::_factories["S"] = std::make_unique<SBlock::Factory>();
	BlockFactory::_factories["Z"] = std::make_unique<ZBlock::Factory>();
	BlockFactory::_factories["T"] = std::make_unique<TBlock::Factory>();
}

std::shared_ptr<Block> BlockFactory::createBlock(const std::string& id, int level)
{
	if( _factories.find(id) != _factories.end() )
		return _factories[id]->create(level);
}

IBlock::IBlock(int level)
{
	_type = BlockType::I;
	_bottomLeft = Coord(0,14);

	_cells.emplace_back(std::make_shared<Cell>(0,14,this,"./assets/t.png",false,(char)BlockType::I));
	_cells.emplace_back(std::make_shared<Cell>(1,14,this,"./assets/t.png",false,(char)BlockType::I));
	_cells.emplace_back(std::make_shared<Cell>(2,14,this,"./assets/t.png",false,(char)BlockType::I));
	_cells.emplace_back(std::make_shared<Cell>(3,14,this,"./assets/t.png",false,(char)BlockType::I));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

JBlock::JBlock(int level)
{
	_type = BlockType::J;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,14,this,"./assets/n.png",false,(char)BlockType::J));
	_cells.emplace_back(std::make_shared<Cell>(0,13,this,"./assets/n.png",false,(char)BlockType::J));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/n.png",false,(char)BlockType::J));
	_cells.emplace_back(std::make_shared<Cell>(2,13,this,"./assets/n.png",false,(char)BlockType::J));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

LBlock::LBlock(int level)
{
	_type = BlockType::L;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,13,this,"./assets/b.png",false,(char)BlockType::L));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/b.png",false,(char)BlockType::L));
	_cells.emplace_back(std::make_shared<Cell>(2,13,this,"./assets/b.png",false,(char)BlockType::L));
	_cells.emplace_back(std::make_shared<Cell>(2,14,this,"./assets/b.png",false,(char)BlockType::L));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

OBlock::OBlock(int level)
{
	_type = BlockType::O;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,13,this,"./assets/y.png",false,(char)BlockType::O));
	_cells.emplace_back(std::make_shared<Cell>(0,14,this,"./assets/y.png",false,(char)BlockType::O));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/y.png",false,(char)BlockType::O));
	_cells.emplace_back(std::make_shared<Cell>(1,14,this,"./assets/y.png",false,(char)BlockType::O));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

SBlock::SBlock(int level)
{
	_type = BlockType::S;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,13,this,"./assets/g.png",false,(char)BlockType::S));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/g.png",false,(char)BlockType::S));
	_cells.emplace_back(std::make_shared<Cell>(1,14,this,"./assets/g.png",false,(char)BlockType::S));
	_cells.emplace_back(std::make_shared<Cell>(2,14,this,"./assets/g.png",false,(char)BlockType::S));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

ZBlock::ZBlock(int level)
{
	_type = BlockType::Z;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,14,this,"./assets/r.png",false,(char)BlockType::Z));
	_cells.emplace_back(std::make_shared<Cell>(1,14,this,"./assets/r.png",false,(char)BlockType::Z));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/r.png",false,(char)BlockType::Z));
	_cells.emplace_back(std::make_shared<Cell>(2,13,this,"./assets/r.png",false,(char)BlockType::Z));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

TBlock::TBlock(int level)
{
	_type = BlockType::T;
	_bottomLeft = Coord(0,13);

	_cells.emplace_back(std::make_shared<Cell>(0,14,this,"./assets/p.png",false,(char)BlockType::T));
	_cells.emplace_back(std::make_shared<Cell>(1,14,this,"./assets/p.png",false,(char)BlockType::T));
	_cells.emplace_back(std::make_shared<Cell>(1,13,this,"./assets/p.png",false,(char)BlockType::T));
	_cells.emplace_back(std::make_shared<Cell>(2,14,this,"./assets/p.png",false,(char)BlockType::T));

	_levelGenerated = level;
	_isHeavy = (level >= 3);
}

BlockType Block::getType(void)
{
	return this->_type;
}

std::vector<std::shared_ptr<Cell>>& Block::getCells(void)
{
	return this->_cells;
}
