#ifndef _BULLET_H_
#define _BULLET_H_

#include "cocos2d.h"
#include "Entity.h"

class Bullet :public Entity
{
public:
	static Bullet* createBullet();

	virtual bool init();

	CREATE_FUNC(Bullet);
};




#endif // !_BULLET_H_
