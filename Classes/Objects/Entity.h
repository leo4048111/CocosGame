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

	static Entity* createEntity();

	CREATE_FUNC(Entity);
	void bindSprite(cocos2d::Sprite* sprite);

	//health related
	double getHealthPercentage();

	void resetHealthBar();

	void showHealthBar();

	bool receiveDamage(double damage);

	void addHealth(double heal);

	void setHealth(double health);

	//speed related
	void addSpeed(double speed);

	double getCurrentSpeed();

	double getSpeedPercentage();

	//stamina related
	double getCurrentStamina();

	void addStamina(double stamina);

	void setStamina(double stamina);

	void addStaminaRecovery(double stamina);

	double getStaminaRecovery();

	double getStaminaPercentage();

	void resetStaminaBar();

	void showStaminaBar();

	//resistance related
	void addResistance(double resistance);

	double getCurrentResistance();

	void setResistance(double resistance);

	//cleanup methods
	void runDeadAction();

	void deadAndCleanUp(Node* node);

	//chat method
	void speak(std::string str);

	bool isMe();

	void setMe(bool value);

private:
	double m_maxHealth;
	double m_currentHealth;
	cocos2d::ProgressTimer* m_healthBar;

	double m_maxStamina;
	double m_currentStamina;
	double m_currentStaminaRecovery;
	cocos2d::ProgressTimer* m_staminaBar;

	double m_currentResistance;

	double m_currentSpeed;

	bool m_isMe;

};


#endif