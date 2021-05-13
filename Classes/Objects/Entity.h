#ifndef _ENTITY_H_
#define _ENTITY_H_

#include"cocos2d.h"

#define ENTITY_MAX_STAMINA 100

//The general features of an object
class Entity :public cocos2d::Node
{
public:
	cocos2d::Sprite* m_sprite;

public:
	virtual bool init();

	void bindSprite(cocos2d::Sprite* sprite);

	//health related
	double getHealthPercentage();

	void resetHealthBar();

	void showHealthBar();

	bool receiveDamage(int damage);

	void healUp(int heal);

	//speed related
	void addSpeed(double speed);

	double getCurrentSpeed();

	//stamina related
	double getCurrentStamina();

	void addStamina(double stamina);

	double getStaminaPercentage();

	void resetStaminaBar();

	void showStaminaBar();

	//cleanup methods
	void runDeadAction();

	void deadAndCleanUp(Node* node);

private:
	double m_maxHealth;
	double m_currentHealth;
	cocos2d::ProgressTimer* m_healthBar;

	double m_maxStamina;
	double m_currentStamina;
	cocos2d::ProgressTimer* m_staminaBar;

	double m_currentSpeed;



};


#endif