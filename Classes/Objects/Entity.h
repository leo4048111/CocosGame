#ifndef _ENTITY_H_
#define _ENTITY_H_

#include"cocos2d.h"

//The general features of an object
class Entity :public cocos2d::Node
{
public:
	cocos2d::Sprite* m_sprite;
	
	cocos2d::ProgressTimer* m_healthBar;

public:
	virtual bool init();

	void bindSprite(cocos2d::Sprite* sprite);

	double getHealthPercentage();

	void resetHealthBar();

	void showHealthBar();

	bool receiveDamage(int damage);

	void deadAndCleanUp(Node* node);
private:
	double m_maxHealth;
	double m_currentHealth;

};


#endif