#ifndef _COLLECTABLE_H_
#define _COLLECTABLE_H_

#include "cocos2d.h"
#include "Entity.h"
#include <ctime>
#include "Player.h"
#include "Weapon.h"

#define MAX_RETAIN_TIME 8

enum collectableType
{
	ammo,health,speed,stamina,resistance, cpistol, clazer, csniperRifle, csawedOff, crifle, cplagueBringer, cflameThrower
};


class Collectable :public Entity
{
public:
	static Collectable* createCollectable();

	virtual bool init();

	CREATE_FUNC(Collectable);

	void setCollectableType(collectableType type);

	//anime 
	bool loadGraphs();

	//type and relative applications
	collectableType getCollectableType();

	//collectable functions
	void useCollectable();

	void addAmmo();

	void addHealth();

	void addSpeed();

	void addStaminaRecovery();

	void addResistance();

	//collectable survival period
	bool isStillValid();
private:
	//collectable specs
	collectableType m_type;

	//time specs
	time_t m_lastUpdateTime;
};

#endif // !_COLLECTABLE_H_
