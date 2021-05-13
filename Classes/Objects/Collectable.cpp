#include "Collectable.h"

USING_NS_CC;

Collectable* Collectable::createCollectable()
{
	return Collectable::create();
}

bool Collectable::init()
{
	if (!Entity::init())
	{
		return false;
	}
	if (!Collectable::loadGraphs())
	{
		return false;
	}

	m_lastUpdateTime = time(NULL);
	Collectable::setCollectableType(ammo);
	return true;
}

void Collectable::setCollectableType(collectableType type)
{
	m_type = type;
	String collectableTypeName;
	//Name format as such: collectable_$TYPE
	switch (m_type)
	{
	case collectableType::ammo:
		collectableTypeName = "collectable_ammo";
		break;
	case collectableType::health:
		collectableTypeName = "collectable_health";
		break;
	case collectableType::speed:
		collectableTypeName = "collectable_speed";
		break; 
	default:
		return;
	}

	//reset target sprite display
	auto target = Sprite::createWithSpriteFrameName(collectableTypeName.getCString());
	if (m_sprite != nullptr)
		m_sprite->removeFromParentAndCleanup(1);
	this->setName(collectableTypeName.getCString());
	target->setScale(0.2f);
	Collectable::bindSprite(target);
}

bool Collectable::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/collectable/collectable.plist");
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}


void Collectable::useCollectable()
{
	switch (this->getCollectableType())
	{
	case collectableType::ammo:
		Collectable::addAmmo();
		break;
	case collectableType::health:
		Collectable::addHealth();
		break;
	case collectableType::speed:
		Collectable::addSpeed();
		break;
	default:
		return;
	}
}

collectableType Collectable::getCollectableType()
{
	return m_type;
}

void Collectable::addAmmo()
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	Weapon* weapon = mainCharacter->getCurrentWeapon();
	weapon->getBackupMagazine();
}

void Collectable::addHealth()
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	mainCharacter->healUp(20);
}

void Collectable::addSpeed()
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	mainCharacter->addSpeed(0.1f);
}

bool Collectable::isStillValid()
{
	time_t currentTime = time(NULL);
	return (currentTime - m_lastUpdateTime) < MAX_RETAIN_TIME;
}