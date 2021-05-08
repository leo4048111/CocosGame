#ifndef _TARGET_H_
#define _TARGET_H_

#include "cocos2d.h"
#include "Entity.h"
#include <ctime>

enum
{
	moveleft,moveright
};

class Target :public Entity
{
public:
	static Target* createTarget();

	virtual bool init();

	CREATE_FUNC(Target);

	//anime 
	bool loadGraphs();

	//call method schedule update
	virtual void update(float delta);

private:
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	int m_currentDir;
	time_t m_lastUpdateTime;
};


#endif // !_TARGET_H_
