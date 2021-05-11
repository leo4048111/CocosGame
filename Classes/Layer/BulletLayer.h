#ifndef _BULLET_LAYER_H
#define _BULLET_LAYER_H

#include "cocos2d.h"

class BulletLayer :public cocos2d::Layer
{
public:
	static BulletLayer* createBulletLayer();

	virtual bool init();

	CREATE_FUNC(BulletLayer);

	void cleanBullet(Node* sender);

	virtual void update(float delta);

	//Shooting specs
	void addBullet();

	void addLazer();
	
private:
	cocos2d::Vector<cocos2d::Sprite*> m_allBullets;
};

#endif // !_BULLET_LAYER_H