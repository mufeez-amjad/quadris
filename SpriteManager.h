#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

#include <string>
#include <memory>
#include <unordered_map>

#include "Sprite2D.h"

class SpriteManager
{
public:
	SpriteManager(const std::string& spriteFile);
	std::shared_ptr<Sprite2D> getSprite(const std::string& spriteFile);

private:
	std::unordered_map<std::string, std::shared_ptr<Sprite2D>> _sprites;
};

#endif // SPRITEMANAGER_H
