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

	//Init visual specs
	m_magazineSpecLabel = Label::create("0/0", "HeiTi", 20);
	m_magazineSpecLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_magazineSpecLabel->setPosition(Vec2(this->getContentSize().width / 2, -m_magazineSpecLabel->getContentSize().height));
	m_magazineSpecLabel->setScale(0.3f);
	this->addChild(m_magazineSpecLabel);

	/*m_weaponSpecMenu = Menu::create();
	m_weaponSpecMenu->setPosition(Vec2(this->getContentSize().width, 0));
	m_weaponSpecMenu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_weaponSpecMenu->alignItemsVerticallyWithPadding(10.0f);
	this->addChild(m_weaponSpecMenu);*/

	//Init weapon
	for (int c = 1; c <= MAX_WEAPON_CARRY; c++)
		m_emptyWeaponSlots.push_back(5-c);
	m_totalWeapons = 0;
	auto weapon = Weapon::createWeapon();
	MainCharacter::addWeapon(weapon);
	weapon->setWeaponType(Weapon::weaponType::razor);
	MainCharacter::swapWeapon(1);
	auto weapon2 = Weapon::createWeapon();
	weapon2->setWeaponType(Weapon::weaponType::pistol);
	MainCharacter::addWeapon(weapon2);

	this->setName("MainCharacter");
	this->setControlOnListen(); 
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
	//Update anime
	double offsetX = 1.0f;
	double offsetY = 1.0f;
	if (m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT])
	{
		offsetX += 0.5f;
		offsetY += 0.5f;
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

	//update visual specs
	if (m_currentWeapon != nullptr)
		m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());
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
		//Add sprite
		m_totalWeapons++;
		int slot = m_emptyWeaponSlots.back();
		m_weaponsMap.insert(std::make_pair(slot, weapon));
		m_emptyWeaponSlots.pop_back();
		m_sprite->addChild(weapon);
		weapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		weapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
		weapon->setVisible(false);
		weapon->setControlOnListen();
		
		////Update Menu
		//auto tmpFont = MenuItemFont::create(weapon->getName());
		//tmpFont->setTag(slot);
		//m_weaponSpecMenu->addChild(tmpFont); // Append the weapon to menu
		//tmpFont->setColor(Color3B(255, 255, 255));
	}
}

void MainCharacter::swapWeapon(int num)
{
	if (m_currentWeapon != m_weaponsMap[num] && num <= m_totalWeapons)
	{
		int previousSlot = m_currentWeaponSlot;
		//Update sprite
		if (m_currentWeapon != nullptr)
		{
			m_currentWeapon->setVisible(false);
			m_currentWeapon->pauseControlListen();
		}
		m_currentWeapon = m_weaponsMap[num];
		m_currentWeapon->setVisible(true);
		m_currentWeapon->resumeControlListen();
		m_currentWeaponSlot = num;

		//Update visual specs
	/*	if (m_weaponSpecMenu->getChildByTag(previousSlot) != nullptr)
		{
			m_weaponSpecMenu->getChildByTag(previousSlot)->setColor(Color3B(255, 255, 255));
			m_weaponSpecMenu->getChildByTag(m_currentWeaponSlot)->setColor(Color3B(255, 0, 0));
		}*/
		m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());



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
			//Update sprite
			m_weaponsMap.erase(m_weaponsMap.find(m_currentWeaponSlot));
			m_currentWeapon->removeFromParent();
			this->getParent()->addChild(m_currentWeapon);
			m_emptyWeaponSlots.push_back(m_currentWeaponSlot);

			//Update spec menu
			m_weaponSpecMenu->getChildByTag(m_currentWeaponSlot)->removeFromParentAndCleanup(true);
		}
	}
}