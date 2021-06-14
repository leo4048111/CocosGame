#ifndef _TARGET_H_
#define _TARGET_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Collectable.h"
#include "../Layer/CollectableLayer.h"
#include <ctime>

enum
{
	moveleft,moveright,attackleft,attackright
};

enum targetType
{
	ghost, jellyGhost, sadGhost, spirit
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

	void moveWithPos(double posX, double posY, int dir);

	void setTargetType(targetType type);

	targetType getTargetType();

	//drop collectables
	void dropRandomCollectable();

	void dropSpecificCollectable(collectableType type);

	//attack funtions!
	void attack(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void attackEnd(Node* sender);

	void fireSpiritualPower(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireFlameCircle(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireSubterrainAssualt(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void doFastStrike(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

private:
	//target anime update scheduler
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*>m_leftAttackAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*>m_rightAttackAnime;
	int m_currentDir;
	time_t m_lastUpdateTime;

	//target specs
	targetType m_type;

	//attack features
	bool m_isAttacking;
	
};


#endif // !_TARGET_H_
