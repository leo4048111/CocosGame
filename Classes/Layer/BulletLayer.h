#ifndef _BULLET_LAYER_H
#define _BULLET_LAYER_H

#include "cocos2d.h"


#define TARGET_BULLET_SPEED 20.0f

class BulletLayer :public cocos2d::Layer
{
public:
	static BulletLayer* createBulletLayer();

	virtual bool init();

	CREATE_FUNC(BulletLayer);

	void cleanBullet(Node* sender);

	virtual void update(float delta);

	//friendly shooting specs
	void addBullet();

	void addLazer();

	void addSprayBullet();

	//hostile shooting specs
	void addSpiritualShockWave(cocos2d::Node* sender);
	
private:
	cocos2d::Vector<cocos2d::Sprite*> m_allFriendlyBullets;
	cocos2d::Vector<cocos2d::Sprite*> m_allHostileBullets;
};

#endif // !_BULLET_LAYER_H