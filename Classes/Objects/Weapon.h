#ifndef _WEAPONS_H_
#define _WEAPONS_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Layer/BulletLayer.h"

enum weaponType
{
	bigKnife,pistol, lazer, sniperRifle, sawedOff, rifle, plagueBringer,flameThrower,totalWeapons
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

	void fire(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void runFireEffect(std::string filePath);

	void fireNormalBullet(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireLazer(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireSprayAmmo(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireToxicBomb(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void fireFlameThrower(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void doMeleeAttack(cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void reload();

	void getBackupMagazine();


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
