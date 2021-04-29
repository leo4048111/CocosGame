#ifndef _TARGET_H_
#define _TARGET_H_

#include "cocos2d.h"
#include "Entity.h"

class Target :public Entity
{
public:
	static Target* createTarget();

	virtual bool init();

	CREATE_FUNC(Target);

	////auto movement update
	//void update(float delta);
};


#endif // !_TARGET_H_
