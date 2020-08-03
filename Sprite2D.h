#ifndef SPRITE2D_H
#define SPRITE2D_H

#include <memory>
#include <string>
#include <QPixmap>

class Sprite2D
{
public:
	Sprite2D();
	Sprite2D(const std::string& asset);
	void loadAsset(const std::string& asset);
	bool hasData(void) const;
	QPixmap getData(void) const;

private:
	std::shared_ptr<QPixmap> _data;
};

#endif // SPRITE2D_H
