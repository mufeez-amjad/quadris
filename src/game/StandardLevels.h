#ifndef STANDARDLEVELS_H
#define STANDARDLEVELS_H

#include <map>

#include "Level.h"

#define MAX_LEVEL 4

class Level0 : public Level
{
public:
	~Level0();
	Level0() {};
	Level0(std::string, Game*, bool, std::shared_ptr<std::ifstream>);
	virtual std::shared_ptr<Block> getNextBlock(int) override;
	virtual void useRandom(bool use) override;

private:
	friend class LevelFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public LevelFactory
	{
		friend class LevelFactoryInitializer;
		public:
			virtual std::unique_ptr<Level> create(std::string 	filePath,
												  Game* 		gameRef,
												  bool 			rand,
												  std::shared_ptr<std::ifstream>
												  				sin) {
				return std::make_unique<Level0>(filePath, gameRef, rand, sin);
			};
	};
};


class Level1 : public Level
{
public:
	~Level1();
	Level1(std::string, Game*, bool, std::shared_ptr<std::ifstream>);
	std::shared_ptr<Block> getNextBlock(int) override;

private:
	friend class LevelFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public LevelFactory
	{
		friend class LevelFactoryInitializer;
		public:
			virtual std::unique_ptr<Level> create(std::string 	filePath,
												  Game* 		gameRef,
												  bool 			rand,
												  std::shared_ptr<std::ifstream>
												  				sin) {
				return std::make_unique<Level1>(filePath, gameRef, rand, sin);
			};
	};
};


class Level2 : public Level
{
public:
	~Level2();
	Level2(std::string, Game*, bool, std::shared_ptr<std::ifstream>);
	std::shared_ptr<Block> getNextBlock(int) override;

private:
	friend class LevelFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public LevelFactory
	{
		friend class LevelFactoryInitializer;
		public:
			virtual std::unique_ptr<Level> create(std::string 	filePath,
												  Game*			gameRef,
												  bool			rand,
												  std::shared_ptr<std::ifstream>
												  				sin) {
				return std::make_unique<Level2>(filePath, gameRef, rand, sin);
			};
	};
};


class Level4;

class Level3 : public Level0
{
public:
	~Level3();
	Level3() {};
	Level3(std::string, Game*, bool, std::shared_ptr<std::ifstream>);
	virtual std::shared_ptr<Block> getNextBlock(int) override;

	void useRandom(bool random) override;

private:
	friend class LevelFactoryInitializer;
	class Factory;
	friend class Factory;
	friend class Level4;

	class Factory : public LevelFactory
	{
		friend class LevelFactoryInitializer;
		public:
			virtual std::unique_ptr<Level> create(std::string	filePath,
												  Game*			gameRef,
												  bool			rand,
												  std::shared_ptr<std::ifstream>
												  				sin) {
				return std::make_unique<Level3>(filePath, gameRef, rand, sin);
			};
	};

	Level0 _level0;
};


class Level4 : public Level3
{
public:
	~Level4();
	Level4(std::string, Game*, bool, std::shared_ptr<std::ifstream>);
	std::shared_ptr<Block> getNextBlock(int) override;

	void useRandom(bool random) override;

private:
	friend class LevelFactoryInitializer;
	class Factory;
	friend class Factory;

	class Factory : public LevelFactory
	{
		friend class LevelFactoryInitializer;
		public:
			virtual std::unique_ptr<Level> create(std::string 	filePath,
												  Game*			gameRef,
												  bool			rand,
												  std::shared_ptr<std::ifstream>
												  				sin) {
				return std::make_unique<Level4>(filePath, gameRef, rand, sin);
			};
	};

	Level3 _level3;
};

#endif // STANDARDLEVELS_H
