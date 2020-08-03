#ifndef STANDARDBLOCKS_H
#define STANDARDBLOCKS_H

#include "Block.h"

class IBlock : public Block
{
public:
	IBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<IBlock>(level);
			};
	};
};

class JBlock : public Block
{
public:
	JBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<JBlock>(level);
			};
	};
};

class LBlock : public Block
{
public:
	LBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<LBlock>(level);
			};
	};
};

class OBlock : public Block
{
public:
	OBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<OBlock>(level);
			};
	};
};

class SBlock : public Block
{
public:
	SBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<SBlock>(level);
			};
	};
};

class ZBlock : public Block
{
public:
	ZBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<ZBlock>(level);
			};
	};
};

class TBlock : public Block
{
public:
	TBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<TBlock>(level);
			};
	};
};


class DBlock : public Block
{
public:
	DBlock(int);

private:
	friend class BlockFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public BlockFactory
	{
			friend class BlockFactoryInitializer;
		public:
			std::shared_ptr<Block> create(int level) {
				return std::make_shared<DBlock>(level);
			};
	};
};

#endif // STANDARDBLOCKS_H
