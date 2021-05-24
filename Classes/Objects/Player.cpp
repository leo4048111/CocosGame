#include "Player.h"
#include "Controls/Specs.h"

USING_NS_CC;

Player* Player::createMainCharacter()
{
	return Player::create();
}

bool Player::init()
{
	if (!Entity::init())
	{
		return false;
	}
	if (!Player::loadGraphs())
	{
		return false;
	}

	//Init sprite
	Sprite* mainCharacter = Sprite::createWithSpriteFrameName("character_maleAdventurer_backwalk0");
	mainCharacter->setScale(0.1f);
	Player::bindSprite(mainCharacter);
	Player::showHealthBar();
	Player::showStaminaBar();

	//Init specs
	m_magazineSpecLabel = Label::create("0/0", "HeiTi", 20);
	m_magazineSpecLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_magazineSpecLabel->setPosition(Vec2(this->getContentSize().width / 2, -m_magazineSpecLabel->getContentSize().height));
	m_magazineSpecLabel->setScale(0.3f);
	this->addChild(m_magazineSpecLabel);
	this->addSpeed(1.0f);
	
	//init weapons
	initAllWeapon();

	//init name
	m_playerName = Specs::getInstance()->getPlayerName();
	auto nameLabel = Label::create(m_playerName,"",7);
	this->addChild(nameLabel);
	nameLabel->setColor(Color3B(255, 255, 255));
	nameLabel->setPosition(Vec2(this->getContentSize().width / 2, 20));

	this->setName(Specs::getInstance()->getPlayerName());
	this->setControlOnListen(); 
	return true;
}

bool Player::loadGraphs()
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
		m_leftSlideAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_leftslide"));
		m_rightSlideAnime.pushBack(cache->getSpriteFrameByName("character_maleAdventurer_rightslide"));

		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

void Player::runActionAnime(int dir)
{
	Animation* anime=nullptr;
	switch (dir)
	{
	case actions::forward:
		anime = Animation::createWithSpriteFrames(m_forwardWalkAnime, 0.3f / 4);
		break;
	case actions::back:
		anime = Animation::createWithSpriteFrames(m_backWalkAnime, 0.3f / 3);
		break;
	case actions::left:
		anime = Animation::createWithSpriteFrames(m_leftWalkAnime, 0.5f / 8);
		break;
	case actions::right:
		anime = Animation::createWithSpriteFrames(m_rightWalkAnime, 0.5f / 8);
		break;
	case actions::standBack:
		anime = Animation::createWithSpriteFrames(m_standBackAnime, 0.5f / 2);
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

void Player::update(float delta)
{
	//Update anime
	double offsetX = this->getCurrentSpeed();
	double offsetY = this->getCurrentSpeed();
	if (m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT]&&this->getCurrentStamina())
	{
		offsetX *= 2.5f;
		offsetY *= 2.5f;
		this->addStamina(-2.0f);
	}
	else if(!m_keyMap[EventKeyboard::KeyCode::KEY_SHIFT])
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

	//update visual specs
	if (m_currentWeapon != nullptr)
		m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());

	//auto fire weapon
	if (m_mouseButtonMap[EventMouse::MouseButton::BUTTON_LEFT] && m_currentWeapon->m_isAutoFire)
		m_currentWeapon->fire();
}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_keyMap[keycode] = true; //set pressed status to true
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		m_sprite->stopActionByTag(standBack);
		runActionAnime(actions::forward);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_sprite->stopActionByTag(standBack);
		runActionAnime(actions::back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_sprite->stopActionByTag(standBack);
		runActionAnime(actions::left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_sprite->stopActionByTag(standBack);
		runActionAnime(actions::right);
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
	case EventKeyboard::KeyCode::KEY_5:
		swapWeapon(5);
		break;
	case EventKeyboard::KeyCode::KEY_6:
		swapWeapon(6);
		break;
	case EventKeyboard::KeyCode::KEY_7:
		swapWeapon(7);
		break;
	case EventKeyboard::KeyCode::KEY_R:
		m_currentWeapon->reload();
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		break;
	default:
		break;
	}
}

void Player::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_keyMap[keycode] = false; //set pressed status to false
	switch(keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		m_sprite->stopActionByTag(actions::forward);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_sprite->stopActionByTag(back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_sprite->stopActionByTag(actions::left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_sprite->stopActionByTag(actions::right);
		break;
	default:
		break;
	}
	if (!(m_keyMap[EventKeyboard::KeyCode::KEY_W] || m_keyMap[EventKeyboard::KeyCode::KEY_S] || m_keyMap[EventKeyboard::KeyCode::KEY_A] || m_keyMap[EventKeyboard::KeyCode::KEY_D]))
		runActionAnime(standBack);
}

void Player::setControlOnListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
	auto* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}

void Player::onMouseDown(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	EventMouse::MouseButton mouseButton = mouseEvent->getMouseButton();
	m_mouseButtonMap[mouseButton] = true;
	switch (mouseButton)
	{
	case EventMouse::MouseButton::BUTTON_LEFT:
		if (m_currentWeapon != nullptr)
			m_currentWeapon->fire();
		break;
	default:
		break;

	}
}

void Player::onMouseUp(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	EventMouse::MouseButton mouseButton = mouseEvent->getMouseButton();
	m_mouseButtonMap[mouseButton] = false;
}

void Player::swapWeapon(int num)
{
		//Update sprite
		if (m_currentWeapon != nullptr)
		{
			m_currentWeapon->setVisible(false);
			m_currentWeapon->pauseControlListen();
		}
		m_currentWeapon = m_allWeaponsMap[num];
		m_currentWeapon->setVisible(true);
		m_currentWeapon->resumeControlListen();
		m_currentWeaponSlot = num;

		m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());
	
}

Weapon* Player::getCurrentWeapon()
{
	return m_currentWeapon;
}

//void MainCharacter::slide()  //deprecated!!
//{
//	time_t currentTime = time(NULL);
//	if (currentTime - m_lastSlideTime >= SLIDE_COOLDOWN)
//	{
//		m_sprite->pauseSchedulerAndActions();
//		Animate* animate = nullptr;
//		Vec2 dst = this->getPosition();
//		if (m_keyMap[EventKeyboard::KeyCode::KEY_A])
//		{
//			animate = Animate::create(Animation::createWithSpriteFrames(m_leftSlideAnime, 0.5f));
//			dst = Vec2(dst.x-m_currentSpeed * 3,dst.y);
//		}
//		else if (m_keyMap[EventKeyboard::KeyCode::KEY_D])
//		{
//			animate = Animate::create(Animation::createWithSpriteFrames(m_rightSlideAnime, 0.5f));
//			dst = Vec2(dst.x + m_currentSpeed * 3, dst.y);
//
//		}
//		else
//		{
//			animate = Animate::create(Animation::createWithSpriteFrames(m_leftSlideAnime, 0.5f));
//			dst = Vec2(dst.x - m_currentSpeed * 3, dst.y);
//		}
//		
//		Action* actionSlide = Repeat::create(animate, 1);
//		auto action = MoveTo::create(0.5f, dst);
//		CallFuncN* callFunc = CallFuncN::create(this,callfuncN_selector(MainCharacter::slideEnd));
//		auto sequence = Sequence::create(action, callFunc, NULL);
//
//		m_sprite->runAction(actionSlide);
//		m_sprite->runAction(sequence);
//		m_lastSlideTime = currentTime;
//	}
//}

void Player::addWeapon(Weapon* weapon)
{
	m_allWeaponsMap.insert(std::make_pair(weapon->getWeaponType()+1, weapon));
	m_sprite->addChild(weapon);
	weapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	weapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
	weapon->setVisible(false);
	weapon->setControlOnListen();
}

void Player::initAllWeapon()
{
	//Init pistol
	auto wpistol = Weapon::createWeapon();
	wpistol->setWeaponType(weaponType::pistol);
	addWeapon(wpistol);
	//Init lazer
	auto wlazer = Weapon::createWeapon();
	wlazer->setWeaponType(weaponType::lazer);
	addWeapon(wlazer);

	//Init sniperRifle
	auto wsniperRifle = Weapon::createWeapon();
	wsniperRifle->setWeaponType(weaponType::sniperRifle);
	addWeapon(wsniperRifle);

	//Init sawedOff
	auto wsawedOff = Weapon::createWeapon();
	wsawedOff->setWeaponType(weaponType::sawedOff);
	addWeapon(wsawedOff);

	//Init rifle
	auto wrifle = Weapon::createWeapon();
	wrifle->setWeaponType(weaponType::rifle);
	addWeapon(wrifle);

	//Init toxicPistol
	auto wplagueBringer = Weapon::createWeapon();
	wplagueBringer->setWeaponType(weaponType::plagueBringer);
	addWeapon(wplagueBringer);

	//Init flameThrower
	auto wflameThrower = Weapon::createWeapon();
	wflameThrower->setWeaponType(weaponType::flameThrower);
	addWeapon(wflameThrower);
	Player::swapWeapon(1);

}