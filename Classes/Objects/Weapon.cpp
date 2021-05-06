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
	m_ammoInCurrentMagazine = 30;
	m_maxAmmoPerMagazine = 30;
	m_backupAmmo = 100;
	this->setName("pistol");
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
	Vec2 weaponPosVec = this->getParent()->getParent()->getPosition();
	Vec2 mousePosVec = Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY());
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians =M_PI-atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	this->setRotation(degree);
}

void Weapon::fire(double dstX, double dstY)
{
	if (m_ammoInCurrentMagazine)
	{
		auto bullet = Sprite::create("objects/ammo/ammo_rifle.png");
		Vec2 mousePosVec = Vec2(dstX, dstY);
		Vec2 weaponPosVec = this->getParent()->getParent()->getPosition();
		Vec2 dst = mousePosVec - weaponPosVec;
		float radians = M_PI - atan2(dst.y, dst.x);
		float degree = CC_RADIANS_TO_DEGREES(radians);
		bullet->setRotation(degree);
		bullet->setScale(0.2f);
		this->getParent()->getParent()->getParent()->addChild(bullet);
		bullet->setPosition(this->getParent()->convertToWorldSpace(this->getPosition())-Vec2(this->getContentSize().width+10-(cos(degree)* this->getContentSize().width+10), sin(degree) * (this->getContentSize().width+10)));
		auto action = MoveTo::create(2.0f, mousePosVec+30*dst);
	/*	CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(cleanBullet,this,bullet));
		auto sequence = Sequence::create(action, callFunc, NULL);*/
		bullet->runAction(Repeat::create(action, 1));
		
		m_ammoInCurrentMagazine--;
	}
}

void Weapon::reload()
{
	if (m_backupAmmo+m_ammoInCurrentMagazine>=m_maxAmmoPerMagazine)
	{
		m_backupAmmo -= m_maxAmmoPerMagazine - m_ammoInCurrentMagazine;
		m_ammoInCurrentMagazine = m_maxAmmoPerMagazine;
	}
}

void Weapon::cleanBullet(Node* node)
{
	Sprite* bullet = dynamic_cast<Sprite*>(node);
	if (bullet != nullptr)
	{
		bullet->stopAllActions();
		bullet->removeFromParentAndCleanup(1);
	}
}