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
	case collectableType::stamina:
		collectableTypeName = "collectable_stamina";
		break;
	case collectableType::resistance:
		collectableTypeName = "collectable_resistance";
		break;
	default:
		return;
	}

	//reset target sprite display
	auto collectable = Sprite::createWithSpriteFrameName(collectableTypeName.getCString());
	if (m_sprite != nullptr)
		m_sprite->removeFromParentAndCleanup(1);
	this->setName(collectableTypeName.getCString());
	collectable->setScale(0.15f);
	Collectable::bindSprite(collectable);
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
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	Label* notification = Label::createWithTTF("blank", "fonts/Notification Font.ttf", 20);
	notification->enableBold();
	switch (this->getCollectableType())
	{
	case collectableType::ammo:
		notification->setString("Ammo++");
		notification->setColor(Color3B(160, 82, 45));
		Collectable::addAmmo();
		break;
	case collectableType::health:
		notification->setString("Health++");
		notification->setColor(Color3B(220, 20, 60));
		Collectable::addHealth();
		break;
	case collectableType::speed:
		notification->setString("Speed++");
		notification->setColor(Color3B(106, 90, 205));
		Collectable::addSpeed();
		break;
	case collectableType::stamina:
		notification->setString("StaminaRegen++");
		notification->setColor(Color3B(127, 255, 0));
		Collectable::addStaminaRecovery();
		break;
	case collectableType::resistance:
		notification->setString("Resistance++");
		notification->setColor(Color3B(139, 0, 0));
		Collectable::addResistance();
		break;
	default:
		return;
	}

	mainCharacter->addChild(notification);
	auto moveto = MoveTo::create(1.5f, Vec2(notification->getPosition().x, notification->getPosition().y + 50));
	auto fadeout = FadeOut::create(1.5f);
	auto spawn = Spawn::create(moveto, fadeout, NULL);
	notification->runAction(spawn);
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

void Collectable::addStaminaRecovery()
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	mainCharacter->addStaminaRecovery(0.1f);
}

void Collectable::addResistance()
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getParent()->getChildByName("MainCharacter"));
	mainCharacter->addResistance(1.0f);
}

bool Collectable::isStillValid()
{
	time_t currentTime = time(NULL);
	return (currentTime - m_lastUpdateTime) < MAX_RETAIN_TIME;
}

