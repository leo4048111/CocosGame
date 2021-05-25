#include "Weapon.h"
#include "Controls/Specs.h"
#include "Layer/UILayer.h"

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
	case weaponType::bigKnife:
		weaponName = "weapon_bigKnife";
		m_damage = 7;
		m_ammoInCurrentMagazine = -1;
		m_maxAmmoPerMagazine = -1;
		m_backupAmmo = -1;
		m_isAutoFire = false;
		break;
	case weaponType::pistol:
		weaponName = "weapon_pistol";
		m_damage = 30;
		m_ammoInCurrentMagazine = 12;
		m_maxAmmoPerMagazine = 12;
		m_backupAmmo = 12;
		m_isAutoFire = false;
		break;
	case weaponType::lazer:
		weaponName = "weapon_razor";
		m_damage = 0.25f;
		m_ammoInCurrentMagazine = 5;
		m_maxAmmoPerMagazine = 5;
		m_backupAmmo = 5;
		m_isAutoFire = false;
		break;
	case weaponType::rifle:
		weaponName = "weapon_rifle";
		m_damage = 5;
		m_ammoInCurrentMagazine = 25;
		m_maxAmmoPerMagazine = 25;
		m_backupAmmo = 25;
		m_isAutoFire = true;
		break;
	case weaponType::sniperRifle:
		weaponName = "weapon_sniperRifle";
		m_damage = 40;
		m_ammoInCurrentMagazine = 8;
		m_maxAmmoPerMagazine = 8;
		m_backupAmmo = 8;
		m_isAutoFire = false;
		break;
	case weaponType::plagueBringer:
		weaponName = "weapon_plagueBringer";
		m_damage = 5;
		m_ammoInCurrentMagazine = 12;
		m_maxAmmoPerMagazine = 12;
		m_backupAmmo = 12;
		m_isAutoFire = false;
		break;
	case weaponType::sawedOff:
		weaponName = "weapon_sawedOff";
		m_damage = 10;
		m_ammoInCurrentMagazine = 5;
		m_maxAmmoPerMagazine = 5;
		m_backupAmmo = 5;
		m_isAutoFire = false;
		break;
	case weaponType::flameThrower:
		weaponName = "weapon_flameThrower";
		m_damage = 0.25f;
		m_ammoInCurrentMagazine = 120;
		m_maxAmmoPerMagazine = 120;
		m_backupAmmo = 120;
		m_isAutoFire = true;
	default:
		return ;
	}

	auto weapon = Sprite::createWithSpriteFrameName(weaponName.getCString());
	if(m_sprite!=nullptr)
	m_sprite->removeFromParentAndCleanup(1);
	this->setName(weaponName.getCString());
	Weapon::bindSprite(weapon);

	m_isLocked = true;
}

bool Weapon::isLocked()
{
	return m_isLocked;
}

void Weapon::unlock()
{
	if (m_isLocked = true)
	{
		m_isLocked = false;
		UILayer::getInstance()->unlockSlot(this->getWeaponType());
	}
	else
	{
		auto notification = Label::createWithTTF("Converted to" +this->getName()+ " ammo", "fonts/Notification Font.ttf", 80);
		notification->setColor(Color3B(255, 4, 56));
		notification->setPosition(Vec2(this->getParent()->getContentSize().width / 2, -2));
		this->getParent()->addChild(notification);
		auto fadein = FadeIn::create(0.25f);
		auto fadeout = FadeOut::create(0.25f);
		auto fadeSequence = Sequence::create(fadein, fadeout, NULL);
		auto repeat = Repeat::create(fadeSequence, 2);
		auto sequence = Sequence::create(repeat, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, notification)), NULL);
		notification->runAction(sequence);
		getBackupMagazine();
	}
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
	if (m_backupAmmo)
	{
		m_backupAmmo -= m_maxAmmoPerMagazine - m_ammoInCurrentMagazine;
		m_ammoInCurrentMagazine = m_maxAmmoPerMagazine;
	}
}

double Weapon::getWeaponDamage()
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
	if (this->getWeaponType() == weaponType::bigKnife)
	{
		doMeleeAttack();
		return;
	}

	if (m_ammoInCurrentMagazine>0||Specs::getInstance()->isInfiniteAmmoActivated())
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
		case weaponType::flameThrower:
			fireFlameThrower();
			break;
		default:
			break;
		}
		if(!Specs::getInstance()->isInfiniteAmmoActivated())
		m_ammoInCurrentMagazine--;
	}
	else
	{
		auto notification = Label::createWithTTF("Press R to reload", "fonts/Notification Font.ttf", 80);
		notification->setColor(Color3B(255, 4, 56));
		notification->setPosition(Vec2(this->getParent()->getContentSize().width / 2, -2));
		this->getParent()->addChild(notification);
		auto fadein = FadeIn::create(0.25f);
		auto fadeout = FadeOut::create(0.25f);
		auto fadeSequence = Sequence::create(fadein, fadeout, NULL);
		auto repeat = Repeat::create(fadeSequence, 2);
		auto sequence = Sequence::create(repeat, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, notification)),NULL);
		notification->runAction(sequence);
	}
}


void Weapon::fireNormalBullet()
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	//create fire anime
	auto biu = Sprite::create("objects/UI/ui_biu.png");
	biu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	biu->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
	biu->setScale(0.2f);
	this->getParent()->addChild(biu, 0, "biu");

	auto fadein = FadeIn::create(0.2f);
	auto fadeout = FadeOut::create(1.0f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, biu));
	auto sequence = Sequence::create(fadein, fadeout, callFunc, NULL);
	biu->runAction(sequence);

	bulletLayer->addBullet();

}

void Weapon::fireLazer()
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	//create fire anime
	auto wow = Sprite::create("objects/UI/ui_wow.png");
	wow->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	wow->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
	wow->setScale(1.0f);
	this->getParent()->addChild(wow, 0, "wow");

	auto fadein = FadeIn::create(0.2f);
	auto fadeout = FadeOut::create(1.0f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, wow));
	auto sequence = Sequence::create(fadein, fadeout, callFunc, NULL);
	wow->runAction(sequence);

	bulletLayer->addLazer();
}

void Weapon::fireSprayAmmo()
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	//create fire anime
	auto biu = Sprite::create("objects/UI/ui_biu.png");
	biu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	biu->setPosition(Vec2(0, this->getParent()->getContentSize().height * 3 / 5));
	biu->setScale(0.2f);
	this->getParent()->addChild(biu, 0, "biu");

	auto fadein = FadeIn::create(0.2f);
	auto fadeout = FadeOut::create(1.0f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, biu));
	auto sequence = Sequence::create(fadein, fadeout, callFunc, NULL);
	biu->runAction(sequence);

	bulletLayer->addSprayBullet();
}

void Weapon::fireToxicBomb()
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();

	bulletLayer->addToxicBomb();
}

void Weapon::fireFlameThrower()
{
	BulletLayer* bulletLayer =BulletLayer::getInstance();

	bulletLayer->addFlameThrower();
}

void Weapon::doMeleeAttack()
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();

	bulletLayer->addMeleeAttack();
}