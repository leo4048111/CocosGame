#ifndef _WEAPONS_H_
#define _WEAPONS_H_

#include "cocos2d.h"
#include "Entity.h"

class Weapon :public Entity
{
public:
	enum weaponType
	{
		rifle, sawedOff, pistol, sniperRifle, razor, toxicPistol
	};

	static Weapon* createWeapon();

	virtual bool init();

	CREATE_FUNC(Weapon);

	bool loadGraphs();

	void setWeaponType(weaponType type);

private:
	weaponType m_type;
};


#endif // !_WEAPONS_H_
