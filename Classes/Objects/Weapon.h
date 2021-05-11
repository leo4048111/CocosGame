#ifndef _WEAPONS_H_
#define _WEAPONS_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Layer/BulletLayer.h"

enum weaponType
{
	rifle, sawedOff, pistol, sniperRifle, lazer, toxicPistol
};

class Weapon :public Entity
{
public:
	//Weapon graph related

	static Weapon* createWeapon();

	virtual bool init();

	CREATE_FUNC(Weapon);

	bool loadGraphs();

	//weapon specs related
	void setWeaponType(weaponType type);

	weaponType getWeaponType();

	//Weapon movement related

	void setControlOnListen();

	void pauseControlListen();

	void resumeControlListen();

	void onMouseMove(cocos2d::Event* event);

	//Ammo and fire related

	void fire();

	void fireNormalBullet();

	void fireLazer();



	void reload();

	void getBackupMagazine();

public:
	//The weapon type
	weaponType m_type;

	//Ammo related
	int m_backupAmmo;
	int m_maxAmmoPerMagazine;
	int m_ammoInCurrentMagazine;
};


#endif // !_WEAPONS_H_
