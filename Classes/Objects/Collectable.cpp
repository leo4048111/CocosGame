#include "Collectable.h"
#include "Controls/Specs.h"

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
	case collectableType::cpistol:
		collectableTypeName = "collectable_pistol";
		break;
	case collectableType::crifle:
		collectableTypeName = "collectable_rifle";
		break;
	case collectableType::cflameThrower:
		collectableTypeName = "collectable_flameThrower";
		break;
	case collectableType::csniperRifle:
		collectableTypeName = "collectable_sniperRifle";
		break;
	case collectableType::cplagueBringer:
		collectableTypeName = "collectable_plagueBringer";
		break;
	case collectableType::csawedOff:
		collectableTypeName = "collectable_sawedOff";
		break;
	case collectableType::clazer:
		collectableTypeName = "collectable_razor";
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
		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}


void Collectable::useCollectable()
{
	Player* player = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	Label* notification = Label::createWithTTF("New weapon!", "fonts/Notification Font.ttf", 20);
	notification->setColor(Color3B(160, 82, 45));
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
	case collectableType::cpistol:
		player->unlockWeapon(weaponType::pistol);
		break;
	case collectableType::csniperRifle:
		player->unlockWeapon(weaponType::sniperRifle);
		break;
	case collectableType::cflameThrower:
		player->unlockWeapon(weaponType::flameThrower);
		break;
	case collectableType::clazer:
		player->unlockWeapon(weaponType::lazer);
		break;
	case collectableType::csawedOff:
		player->unlockWeapon(weaponType::sawedOff);
		break;
	case collectableType::cplagueBringer:
		player->unlockWeapon(weaponType::plagueBringer);
		break;
	default:
		return;
	}

	player->addChild(notification);
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
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	Weapon* weapon = mainCharacter->getCurrentWeapon();
	weapon->getBackupMagazine();
}

void Collectable::addHealth()
{
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	mainCharacter->healUp(20);
}

void Collectable::addSpeed()
{
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	mainCharacter->addSpeed(0.1f);
}

void Collectable::addStaminaRecovery()
{
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	mainCharacter->addStaminaRecovery(0.1f);
}

void Collectable::addResistance()
{
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	mainCharacter->addResistance(1.0f);
}

bool Collectable::isStillValid()
{
	time_t currentTime = time(NULL);
	return (currentTime - m_lastUpdateTime) < MAX_RETAIN_TIME;
}

