#ifndef _WEAPONS_H_
#define _WEAPONS_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Layer/BulletLayer.h"

class Weapon :public Entity
{
public:
	enum weaponType
	{
		rifle, sawedOff, pistol, sniperRifle, razor, toxicPistol
	};

	//Weapon graph related

	static Weapon* createWeapon();

	virtual bool init();

	CREATE_FUNC(Weapon);

	bool loadGraphs();

	void setWeaponType(weaponType type);

	//Weapon movement related

	void setControlOnListen();

	void pauseControlListen();

	void resumeControlListen();

	void onMouseMove(cocos2d::Event* event);

	//Ammo and fire related

	void fire();

	void reload();

public:
	//The weapon type
	weaponType m_type;

	//Ammo related
	int m_backupAmmo;
	int m_maxAmmoPerMagazine;
	int m_ammoInCurrentMagazine;
	cocos2d::Vector<cocos2d::Sprite*> m_allBullets;
};


#endif // !_WEAPONS_H_
