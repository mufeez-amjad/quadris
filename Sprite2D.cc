#include "Sprite2D.h"

Sprite2D::Sprite2D()
	: _data{ nullptr }
{

}

Sprite2D::Sprite2D(const std::string& asset)
{
	this->loadAsset(asset);
}

void Sprite2D::loadAsset(const std::string& asset)
{
	this->_data = std::make_shared<QPixmap>(asset.c_str());
}

bool Sprite2D::hasData(void) const
{
	return this->_data != nullptr;
}

QPixmap Sprite2D::getData(void) const
{
	return *this->_data.get();
}
