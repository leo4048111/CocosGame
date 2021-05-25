#ifndef _WEAPONS_H_
#define _WEAPONS_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Layer/BulletLayer.h"

enum weaponType
{
	bigKnife,pistol, lazer, sniperRifle, sawedOff, rifle, plagueBringer,flameThrower
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

	double getWeaponDamage();

	//Weapon movement related

	void setControlOnListen();

	void pauseControlListen();

	void resumeControlListen();

	void onMouseMove(cocos2d::Event* event);

	//Ammo and fire related

	void fire();

	void fireNormalBullet();

	void fireLazer();

	void fireSprayAmmo();

	void fireToxicBomb();

	void fireFlameThrower();

	void reload();

	void getBackupMagazine();

	void doMeleeAttack();

	//weapon status control
	bool isLocked();

	void unlock();

public:
	//The weapon specs
	weaponType m_type;
	double m_damage;
	bool m_isAutoFire;

	//Ammo related
	int m_backupAmmo;
	int m_maxAmmoPerMagazine;
	int m_ammoInCurrentMagazine;

	//weapon status control
	bool m_isLocked;
};


#endif // !_WEAPONS_H_
