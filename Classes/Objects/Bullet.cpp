#include "Bullet.h"
#include "cocos2d.h"



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

	return true;
}