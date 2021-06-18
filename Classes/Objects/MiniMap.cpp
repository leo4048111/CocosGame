#include "MiniMap.h"

USING_NS_CC;

MiniMap* MiniMap::_instance = NULL;

MiniMap* MiniMap::createMiniMap()
{
	return MiniMap::create();
}

bool MiniMap::init()
{
	if (!Node::init())
	{
		return false;
	}
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	_map = TMXTiledMap::create("map/Map.tmx");
	_map->setScale(0.07f);
	this->addChild(_map);
	
	_Me = Sprite::create("objects/UI/ui_me.png");
	_map->addChild(_Me);
	_Me->setScale(2.0f);

	return true;
}

void MiniMap::UpdateMe(Vec2 newPos)
{
	_Me->setPosition(newPos);
}

void MiniMap::addTarget(int tag,Vec2 pos)
{
	auto icon = Sprite::create("objects/UI/ui_target.png");
	_map->addChild(icon);
	icon->setScale(1.5f);
	icon->setTag(tag);
	icon->setPosition(pos);
}

void MiniMap::updateTarget(int tag, Vec2 newPos)
{
	auto icon = _map->getChildByTag(tag);
	auto moveto=MoveTo::create(0.5f,newPos);
	if (icon != nullptr)
		icon->setPosition(newPos);
}

void MiniMap::removeTarget(int tag)
{
	auto icon = _map->getChildByTag(tag);
	if (icon != nullptr)
		icon->removeFromParent();
}