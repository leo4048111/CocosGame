#ifndef _ENTITY_H_
#define _ENTITY_H_

#include"cocos2d.h"

class Entity :public cocos2d::Node
{
public:
	cocos2d::Sprite* m_sprite;
	
	cocos2d::ProgressTimer* m_healthBar;

public:
	virtual bool init();

	void bindSprite(cocos2d::Sprite* sprite);

	double getHealthPercentage();

	void setCurrentHealth(double health);

	void showHealthBar();


private:
	double m_maxHealth;
	double m_currentHealth;

};


#endif