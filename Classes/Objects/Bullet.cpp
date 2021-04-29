#include "Bullet.h"
#include "cocos2d.h"

USING_NS_CC;

Bullet* Bullet::createBullet()
{
	return Bullet::create();
}

bool Bullet::init()
{
	if (!Entity::init())
	{
		return false;
	}
	
	auto ammo = Sprite::create("objects/ammo/ammo_rifle.png");
	Bullet::bindSprite(ammo);
	return true;
}