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
	case collectableType::sipOfHealth:
		collectableTypeName = "collectable_sipOfHealth";
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
	case collectableType::sipOfHealth:
		Collectable::addHealth();
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
	//DEBUG 
	std::string str1 = this->getParent()->getName()+"\n";
	OutputDebugString(str1.c_str());
	std::string str2 = this->getParent()->getParent()->getName() + "\n";
	OutputDebugString(str2.c_str());

	//DEBUG END
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	mainCharacter->healUp(20);
}

bool Collectable::isStillValid()
{
	time_t currentTime = time(NULL);
	return (currentTime - m_lastUpdateTime) < MAX_RETAIN_TIME;
}