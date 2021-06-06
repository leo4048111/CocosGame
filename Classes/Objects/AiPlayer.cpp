#include "AiPlayer.h"
#include "Controls/Specs.h"
#include "Layer/SpriteLayer.h"
#include "Algorithm/msws.h"

USING_NS_CC;

std::string AiPlayer::m_playerName = "AI";

AiPlayer* AiPlayer::createPlayer(std::string name)
{
	m_playerName = name;
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
	Sprite* player = Sprite::createWithSpriteFrameName("ai_player_backwalk_0");
	AiPlayer::bindSprite(player);
	player->setScale(0.1f);
	AiPlayer::showHealthBar();
	AiPlayer::showStaminaBar();

	//init weapons
	initRandomWeapon();

	//init name
	auto nameLabel = Label::createWithTTF(m_playerName, "fonts/HashedBrowns-WyJgn.ttf", 7);
	this->addChild(nameLabel);
	nameLabel->setColor(Color3B(255, 255, 255));
	nameLabel->setPosition(Vec2(this->getContentSize().width / 2, 20));

	//init update
	m_lastUpdate = time(NULL);

	this->addSpeed(1.0f);

	this->setName("testAi");
	return true;
}

bool AiPlayer::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/player/ai_player.plist");
		auto cache = SpriteFrameCache::getInstance();
		for (int c = 0; c <= 3; c++)
		{
			m_rightWalkAnime.pushBack(cache->getSpriteFrameByName("ai_player_rightwalk_" + Value(c).asString()));
		}
		for (int c = 0; c <= 3; c++)
		{
			m_leftWalkAnime.pushBack(cache->getSpriteFrameByName("ai_player_leftwalk_" + Value(c).asString()));
		}
		for (int c = 0; c <= 3; c++)
		{
			m_backWalkAnime.pushBack(cache->getSpriteFrameByName("ai_player_backwalk_" + Value(c).asString()));
		}
		for (int c = 0; c <= 3; c++)
		{
			m_forwardWalkAnime.pushBack(cache->getSpriteFrameByName("ai_player_forwardwalk_" + Value(c).asString()));
		}

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
	if (currentTime - m_lastUpdate>=2.0f)
	{
		AiControlAutoUpdate();
		auto allTargets = SpriteLayer::getInstance()->getAllTargets();
		Vec2 startPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
		Vec2 terminalPos = allTargets.front()->getParent()->convertToWorldSpace(allTargets.front()->getPosition());
		m_currentWeapon->pointTo(startPos, terminalPos);
		m_currentWeapon->fire(startPos, terminalPos);
		if (m_currentWeapon->isCurrentMagazineNotEmpty())
		{
			m_currentWeapon->getBackupMagazine();
			m_currentWeapon->reload();
		}
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
	m_currentWeapon->setWeaponType((weaponType)(random() % weaponType::totalWeapons));
	m_currentWeapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	m_currentWeapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
}

void AiPlayer::AiControlAutoUpdate()
{
	//simulate keyboard events
	m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT] = random() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_W] = random() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_A] = random() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_S] = random() % 2;
	m_keyMap[EventKeyboard::KeyCode::KEY_D] = random() % 2;

	if (m_keyMap[EventKeyboard::KeyCode::KEY_W])
	{
		this->stopActionByTag(actions::back);
		runActionAnime(actions::forward);
	}
	else
	{
		this->stopActionByTag(actions::forward);
	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_S])
	{
		this->stopActionByTag(actions::forward);
		runActionAnime(actions::back);
	}
	else
	{
		this->stopActionByTag(actions::back);

	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_A])
	{
		this->stopActionByTag(actions::right);
		runActionAnime(actions::left);
	}
	else
	{
		this->stopActionByTag(actions::left);
	}
	if (m_keyMap[EventKeyboard::KeyCode::KEY_D])
	{
		this->stopActionByTag(actions::left);
		runActionAnime(actions::right);
	}
	else
	{
		this->stopActionByTag(actions::right);
	}

}

void AiPlayer::runActionAnime(int dir)
{
	Animation* anime = nullptr;
	switch (dir)
	{
	case actions::forward:
		anime = Animation::createWithSpriteFrames(m_forwardWalkAnime, 0.3f / 4);
		break;
	case actions::back:
		anime = Animation::createWithSpriteFrames(m_backWalkAnime, 0.3f / 4);
		break;
	case actions::left:
		anime = Animation::createWithSpriteFrames(m_leftWalkAnime, 0.5f / 4);
		break;
	case actions::right:
		anime = Animation::createWithSpriteFrames(m_rightWalkAnime, 0.5f / 4);
		break;

	default:
		break;
	}

	if (anime != nullptr)
	{
		auto action = RepeatForever::create(Animate::create(anime));
		action->setTag(dir);
		m_sprite->runAction(action);
	}
}