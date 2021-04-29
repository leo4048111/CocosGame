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

	void fire(double dstX,double dstY);

	void reload();

	void cleanBullet(cocos2d::Node* node);

private:
	//The weapon type
	weaponType m_type;

	//Ammo related
	int m_totalAmmo;
	int m_maxAmmoPerMagazine;
	int m_ammoInCurrentMagazine;
};


#endif // !_WEAPONS_H_
