#include "AiPlayer.h"
#include "Controls/Specs.h"
#include "Layer/SpriteLayer.h"
#include "Algorithm/msws.h"

USING_NS_CC;

AiPlayer* AiPlayer::createPlayer()
{
	return AiPlayer::create();
}

bool AiPlayer::init()
{
	if (!Entity::init())
	{
		return false;
	}
	if (!AiPlayer::loadGraphs())
	{
		return false;
	}

	//Init sprite
	Sprite* player = Sprite::createWithSpriteFrameName("character_maleAdventurer_backwalk0");
	player->setScale(0.1f);
	AiPlayer::bindSprite(player);
	AiPlayer::showHealthBar();
	AiPlayer::showStaminaBar();

	//init weapons
	initRandomWeapon();

	//init name
	m_playerName ="testAi";
	auto nameLabel = Label::createWithTTF(m_playerName, "fonts/HashedBrowns-WyJgn.ttf", 7);
	this->addChild(nameLabel);
	nameLabel->setColor(Color3B(255, 255, 255));
	nameLabel->setPosition(Vec2(this->getContentSize().width / 2, 20));

	//init update
	m_lastUpdate = time(NULL);

	this->setName("testAi");
	return true;
}

bool AiPlayer::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/player/mainCharacter.plist");
		auto cache = SpriteFrameCache::getInstance();
		for (int c = 0; c <= 7; c++)
		{
			m_rightWalkAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_rightwalk" + Value(c).asString()));
		}

		for (int c = 0; c <= 7; c++)
		{
			m_leftWalkAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_leftwalk" + Value(c).asString()));
		}

		for (int c = 0; c <= 3; c++)
		{
			m_forwardWalkAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_forwardwalk" + Value(c).asString()));
		}

		for (int c = 0; c <= 2; c++)
		{
			m_backWalkAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_backwalk" + Value(c).asString()));
		}

		for (int c = 0; c <= 1; c++)
		{
			m_standBackAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_standforward" + Value(c).asString()));
		}
		m_leftSlideAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_leftslide"));
		m_rightSlideAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_rightslide"));
		m_parachuteAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_parachute"));

		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

void AiPlayer::update(float delta)
{
	time_t currentTime = time(NULL);
	if (currentTime - m_lastUpdate>=1.0f)
	{
		AiControlAutoUpdate();
		auto allTargets = SpriteLayer::getInstance()->getAllTargets();
		m_currentWeapon->fire(m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition()), allTargets->front()->getParent()->convertToWorldSpace(allTargets->front()->getPosition()));
		m_lastUpdate = currentTime;
	}

	//Update anime
	double offsetX = this->getCurrentSpeed();
	double offsetY = this->getCurrentSpeed();
	if (m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT] && this->getCurrentStamina())
	{
		offsetX *= 2.5f;
		offsetY *= 2.5f;
		this->addStamina(-2.0f);
	}
	else if (!m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT])
	{
		this->addStamina(this->getStaminaRecovery());
	}

	if (m_keyMap[EventKeyboard::KeyCode::KEY_W])
	{
		this->setPosition(this->getPosition() + Vec2(0, offsetY));
	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_S])
	{
		this->setPosition(this->getPosition() - Vec2(0, offsetY));
	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_A])
	{
		this->setPosition(this->getPosition() - Vec2(offsetX, 0));
	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_D])
	{
		this->setPosition(this->getPosition() + Vec2(offsetX, 0));
	}
}


Weapon* AiPlayer::getCurrentWeapon()
{
	return m_currentWeapon;
}

void AiPlayer::initRandomWeapon()
{
	srand((unsigned int)time(NULL));
	m_currentWeapon = Weapon::createWeapon();
	m_sprite->addChild(m_currentWeapon);
	m_currentWeapon->setWeaponType((weaponType)(rand() % weaponType::totalWeapons));
	m_currentWeapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_currentWeapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
}

void AiPlayer::AiControlAutoUpdate()
{
	//simulate keyboard events
	srand((unsigned int)time(NULL));
	m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT] = msws() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_W] = msws() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_A] = rand() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_S] = msws() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_D] = rand() % 2;

}