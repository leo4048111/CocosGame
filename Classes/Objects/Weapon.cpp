#include "Weapon.h"

USING_NS_CC;

bool Weapon::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/weapon/weapon.plist");
		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

Weapon* Weapon::createWeapon()
{
	return Weapon::create();
}

bool Weapon::init()
{
	if (!Entity::init())
	{
		return false;
	}
	
	if (!Weapon::loadGraphs())
	{
		return false;
	}

	Weapon::setWeaponType(pistol);
	return true;
}

void Weapon::setWeaponType(weaponType type)
{
	m_type = type;
	String weaponName;
	switch (m_type)
	{
	case weaponType::pistol:
		weaponName = "weapon_pistol";
		break;
	case weaponType::razor:
		weaponName = "weapon_razor";
		break;
	case weaponType::rifle:
		weaponName = "weapon_rifle";
		break;
	case weaponType::sniperRifle:
		weaponName = "weapon_sniperRifle";
		break;
	case weaponType::toxicPistol:
		weaponName = "weapon_toxicPistol";
		break;
	case weaponType::sawedOff:
		weaponName = "weapon_sawedOff";
		break;
	default:
		return ;
	}

	auto weapon = Sprite::createWithSpriteFrameName(weaponName.getCString());
	if(m_sprite!=nullptr)
	m_sprite->removeFromParentAndCleanup(1);
	Weapon::bindSprite(weapon);
}