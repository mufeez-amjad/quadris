#include "SpriteManager.h"

#include <fstream>

// Loads sprite asset files specified per line in spriteFile
SpriteManager::SpriteManager(const std::string& spriteFile)
{
	std::ifstream infile(spriteFile);
	std::string asset;
	while (infile >> asset) {
		std::shared_ptr<Sprite2D> sprite = std::make_shared<Sprite2D>(asset);
		this->_sprites[asset] = sprite;
	}
}

// Retrieve a sprite based off of the sprite's asset file
std::shared_ptr<Sprite2D> SpriteManager::getSprite(const std::string&
												   spriteFile)
{
	if (this->_sprites.find(spriteFile) == this->_sprites.end())
		return nullptr;

	return this->_sprites.at(spriteFile);
}
