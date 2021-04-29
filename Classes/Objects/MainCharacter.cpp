#include "MainCharacter.h"

USING_NS_CC;

MainCharacter* MainCharacter::createMainCharacter()
{
	return MainCharacter::create();
}


bool MainCharacter::init()
{
	if (!Entity::init())
	{
		return false;
	}
	if (!MainCharacter::loadGraphs())
	{
		return false;
	}
	//Init sprite
	Sprite* mainCharacter = Sprite::createWithSpriteFrameName("character_maleAdventurer_backwalk0");
	mainCharacter->setScale(0.1f);
	MainCharacter::bindSprite(mainCharacter);
	MainCharacter::showHealthBar();

	//Init weapon
	for (int c = 1; c <= MAX_WEAPON_CARRY; c++)
		m_emptyWeaponSlots.push_back(5-c);
	m_totalWeapons = 0;
	auto weapon = Weapon::createWeapon();
	MainCharacter::addWeapon(weapon);
	MainCharacter;; swapWeapon(1);
	weapon->setWeaponType(Weapon::weaponType::razor);
	auto weapon2 = Weapon::createWeapon();
	MainCharacter::addWeapon(weapon2);
	weapon2->setWeaponType(Weapon::weaponType::pistol);
	return true;
	
}

bool MainCharacter::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/mainCharacter/mainCharacter.plist");
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
		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

void MainCharacter::runAction(int dir)
{
	Animation* anime=nullptr;
	switch (dir)
	{
	case forward:
		anime = Animation::createWithSpriteFrames(m_forwardWalkAnime, 0.3f / 4);
		break;
	case back:
		anime = Animation::createWithSpriteFrames(m_backWalkAnime, 0.3f / 3);
		break;
	case left:
		anime = Animation::createWithSpriteFrames(m_leftWalkAnime, 0.5f / 8);
		break;
	case right:
		anime = Animation::createWithSpriteFrames(m_rightWalkAnime, 0.5f / 8);
		break;
	case standBack:
		anime = Animation::createWithSpriteFrames(m_standBackAnime, 0.5f / 8);
		break;
	case standForward:
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

void MainCharacter::update(float delta)
{
	double offsetX = 2.0f;
	double offsetY = 2.0f;
	if (m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT])
	{
		offsetX += 1.0;
		offsetY += 1.0;
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

void MainCharacter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_keyMap[keycode] = true; //set pressed status to true
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		runAction(forward);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		runAction(back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		runAction(left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		runAction(right);
		break;
	case EventKeyboard::KeyCode::KEY_1:
		swapWeapon(1);
		break;
	case EventKeyboard::KeyCode::KEY_2:
		swapWeapon(2);
		break;
	case EventKeyboard::KeyCode::KEY_3:
		swapWeapon(3);
		break;
	case EventKeyboard::KeyCode::KEY_4:
		swapWeapon(4);
		break;
	case EventKeyboard::KeyCode::KEY_R:
		m_currentWeapon->reload();
		break;
	case EventKeyboard::KeyCode::KEY_G:
		dropWeapon();
		break;
	default:
		break;
	}
}

void MainCharacter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_keyMap[keycode] = false; //set pressed status to false
	switch(keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		m_sprite->stopActionByTag(forward);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_sprite->stopActionByTag(back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_sprite->stopActionByTag(left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_sprite->stopActionByTag(right);
		break;
	default:
		break;
	}
}

void MainCharacter::setControlOnListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* keyListener = EventListenerKeyboard::create();
	auto* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(MainCharacter::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(MainCharacter::onMouseUp, this);
	keyListener->onKeyPressed = CC_CALLBACK_2(MainCharacter::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(MainCharacter::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void MainCharacter::onMouseDown(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	EventMouse::MouseButton mouseButton = mouseEvent->getMouseButton();
	m_mouseButtonMap[mouseButton] = true;
	switch (mouseButton)
	{
	case EventMouse::MouseButton::BUTTON_LEFT:
		if (m_currentWeapon != nullptr)
			m_currentWeapon->fire(mouseEvent->getCursorX(),mouseEvent->getCursorY());
		break;
	default:
		break;

	}
}

void MainCharacter::onMouseUp(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	EventMouse::MouseButton mouseButton = mouseEvent->getMouseButton();
	m_mouseButtonMap[mouseButton] = false;
}

void MainCharacter::addWeapon(Weapon* weapon)
{
	if (MainCharacter::canCarryMoreWeapons())
	{
		m_totalWeapons++;
		m_weapons.insert(std::make_pair(m_emptyWeaponSlots.back(), weapon));
		m_emptyWeaponSlots.pop_back();
		m_sprite->addChild(weapon);
		weapon->setScale(0.5f);
		weapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		weapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
		weapon->setVisible(false);
		weapon->setControlOnListen();
	}
}

void MainCharacter::swapWeapon(int num)
{
	if (m_currentWeapon != m_weapons[num]&&num<= m_totalWeapons)
	{
		if(m_currentWeapon!=nullptr)
		m_currentWeapon->setVisible(false);
		m_currentWeapon->pauseControlListen();
		m_currentWeapon = m_weapons[num];
		m_currentWeapon->setVisible(true);
		m_currentWeapon->resumeControlListen();
	}
}

bool MainCharacter::canCarryMoreWeapons()
{
	return m_emptyWeaponSlots.size() != 0;
}

void MainCharacter::pickUpWeapon()
{

}

void MainCharacter::dropWeapon()
{
	if (!canCarryMoreWeapons())
	{
		if (m_currentWeapon != nullptr)
		{
			m_currentWeapon->removeFromParent();
			this->getParent()->addChild(m_currentWeapon);
		}
	}
}