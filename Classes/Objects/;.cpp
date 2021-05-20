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
	this->setScale(0.5f);
	this->setName("weapon_pistol");
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
		m_damage = 30;
		m_ammoInCurrentMagazine = 12;
		m_maxAmmoPerMagazine = 12;
		m_backupAmmo = 200;
		m_isAutoFire = false;
		break;
	case weaponType::lazer:
		weaponName = "weapon_razor";
		m_damage = 10;
		m_ammoInCurrentMagazine = 100;
		m_maxAmmoPerMagazine = 100;
		m_backupAmmo = 300;
		m_isAutoFire = false;
		break;
	case weaponType::rifle:
		weaponName = "weapon_rifle";
		m_damage = 20;
		m_ammoInCurrentMagazine = 30;
		m_maxAmmoPerMagazine = 30;
		m_backupAmmo = 120;
		m_isAutoFire = true;
		break;
	case weaponType::sniperRifle:
		weaponName = "weapon_sniperRifle";
		m_damage = 50;
		m_ammoInCurrentMagazine = 8;
		m_maxAmmoPerMagazine = 8;
		m_backupAmmo = 24;
		m_isAutoFire = false;
		break;
	case weaponType::plagueBringer:
		weaponName = "weapon_plagueBringer";
		m_damage = 5;
		m_ammoInCurrentMagazine = 12;
		m_maxAmmoPerMagazine = 12;
		m_backupAmmo = 36;
		m_isAutoFire = false;
		break;
	case weaponType::sawedOff:
		weaponName = "weapon_sawedOff";
		m_damage = 10;
		m_ammoInCurrentMagazine = 5;
		m_maxAmmoPerMagazine = 5;
		m_backupAmmo = 20;
		m_isAutoFire = false;
		break;
	case weaponType::flameThrower:
		weaponName = "weapon_flameThrower";
		m_damage = 1;
		m_ammoInCurrentMagazine = 500;
		m_maxAmmoPerMagazine = 500;
		m_backupAmmo = 500;
		m_isAutoFire = true;
	default:
		return ;
	}

	auto weapon = Sprite::createWithSpriteFrameName(weaponName.getCString());
	if(m_sprite!=nullptr)
	m_sprite->removeFromParentAndCleanup(1);
	this->setName(weaponName.getCString());
	Weapon::bindSprite(weapon);
}

void Weapon::setControlOnListen()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(Weapon::onMouseMove, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener,this);
}

void Weapon::pauseControlListen()
{
	Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this, false);
}

void Weapon::resumeControlListen()
{
	Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this, false);
}

void Weapon::onMouseMove(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	Vec2 weaponPosVec = this->getParent()->getParent()->convertToWorldSpaceAR(this->getParent()->getPosition());
	Vec2 mousePosVec = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians =M_PI-atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	this->setRotation(degree);
}

void Weapon::reload()
{
	if (m_backupAmmo+m_ammoInCurrentMagazine>=m_maxAmmoPerMagazine)
	{
		m_backupAmmo -= m_maxAmmoPerMagazine - m_ammoInCurrentMagazine;
		m_ammoInCurrentMagazine = m_maxAmmoPerMagazine;
	}
}

int Weapon::getWeaponDamage()
{
	return m_damage;
}

weaponType Weapon::getWeaponType()
{
	return m_type;
}

void Weapon::getBackupMagazine()
{
	m_backupAmmo += m_maxAmmoPerMagazine;
}

void Weapon::fire()
{
	switch (this->getWeaponType())
	{
	case weaponType::pistol:
		fireNormalBullet();
		break;
	case weaponType::lazer:
		fireLazer();
		break;
	case weaponType::sniperRifle:
		fireNormalBullet();
		break;
	case weaponType::sawedOff:
		fireSprayAmmo();
		break;
	case weaponType::rifle:
		fireNormalBullet();
break;
	case weaponType::plagueBringer:
		fireToxicBomb();
		break;
	default:
		break;
	}
}

void Weapon::fireNormalBullet()
{
	if (m_ammoInCurrentMagazine)
	{
		BulletLayer* bulletLayer = dynamic_cast<BulletLayer*>(this->getParent()->getParent()->getParent()->getParent()->getChildByName("BulletLayer"));
		//create fire anime
		auto biu = Sprite::create("objects/UI/ui_biu.png");
		biu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		biu->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
		biu->setScale(0.2f);
		this->getParent()->addChild(biu, 0, "biu");

		bulletLayer->addBullet();

		m_ammoInCurrentMagazine--;
	}
}

void Weapon::fireLazer()
{
	if (m_ammoInCurrentMagazine)
	{
		BulletLayer* bulletLayer = dynamic_cast<BulletLayer*>(this->getParent()->getParent()->getParent()->getParent()->getChildByName("BulletLayer"));
		//create fire anime
		auto biu = Sprite::create("objects/UI/ui_wow.png");
		biu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		biu->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
		biu->setScale(1.0f);
		this->getParent()->addChild(biu, 0, "biu");

		bulletLayer->addLazer();

		m_ammoInCurrentMagazine--;
	}
}

void Weapon::fireSprayAmmo()
{
	if (m_ammoInCurrentMagazine)
	{
		BulletLayer* bulletLayer = dynamic_cast<BulletLayer*>(this->getParent()->getParent()->getParent()->getParent()->getChildByName("BulletLayer"));
		//create fire anime
		auto biu = Sprite::create("objects/UI/ui_biu.png");
		biu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		biu->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
		biu->setScale(0.2f);
		this->getParent()->addChild(biu, 0, "biu");

		bulletLayer->addSprayBullet();

		m_ammoInCurrentMagazine--;
	}
}

void Weapon::fireToxicBomb()
{
	if (m_ammoInCurrentMagazine)
	{
		BulletLayer* bulletLayer = dynamic_cast<BulletLayer*>(this->getParent()->getParent()->getParent()->getParent()->getChildByName("BulletLayer"));

		bulletLayer->addToxicBomb();
		m_ammoInCurrentMagazine--;
	}
}