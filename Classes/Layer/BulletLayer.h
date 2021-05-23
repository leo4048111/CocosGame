#ifndef _BULLET_LAYER_H
#define _BULLET_LAYER_H

#include "cocos2d.h"


#define TARGET_BULLET_SPEED 10.0f

class BulletLayer :public cocos2d::Layer
{
public:
	static BulletLayer* createBulletLayer();

	virtual bool init();

	CREATE_FUNC(BulletLayer);

	void cleanBullet(Node* sender);

	virtual void update(float delta);

	/*friendly shooting specs
	All PNG sources should be placed facing left*/
	void addBullet();

	void addLazer();

	void addSprayBullet();

	void addToxicBomb();

	void addFlameThrower();

	//hostile shooting specs
	void addSpiritualPower(cocos2d::Node* sender);

	void addFlameCircle(cocos2d::Node* sender);
	
private:
	cocos2d::Vector<cocos2d::Sprite*> m_allFriendlyBullets;
	cocos2d::Vector<cocos2d::Sprite*> m_allHostileBullets;
};

#endif // !_BULLET_LAYER_H