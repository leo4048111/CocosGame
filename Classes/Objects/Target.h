#ifndef _TARGET_H_
#define _TARGET_H_

#include "cocos2d.h"
#include "Entity.h"
#include <ctime>

enum
{
	moveleft,moveright
};

enum targetType
{
	ghost
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

	void setTargetType(targetType type);

	targetType getTargetType();

private:
	//target anime update scheduler
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	int m_currentDir;
	time_t m_lastUpdateTime;

	//target specs
	targetType m_type;
	
};


#endif // !_TARGET_H_
