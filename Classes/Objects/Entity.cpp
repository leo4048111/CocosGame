#include "Entity.h"

USING_NS_CC;

void Entity::bindSprite(Sprite* sprite)
{
	m_sprite = sprite;
	this->addChild(m_sprite);
}

bool Entity::init()
{
	if (!Node::init())
	{
		return false;
	}
	return true;
}