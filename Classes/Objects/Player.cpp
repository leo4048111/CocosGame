#include "Player.h"
#include "Controls/Specs.h"
#include "Objects/CrossHair.h"
#include "Layer/SpriteLayer.h"
#include "Network/SocketClient.h"

USING_NS_CC;

Player* Player::createPlayer(std::string name)
{
	Player* player = Player::create();
	player->m_playerName = name;
	Label* label = dynamic_cast<Label*>(player->getChildByTag(100));
	if (label != nullptr)
	{
		label->setString(name);
	}
	return player;
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
	m_magazineSpecLabel = Label::createWithTTF("0/0", "fonts/Arial.ttf", 20);
	m_magazineSpecLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	m_magazineSpecLabel->setPosition(Vec2(this->getContentSize().width / 2, -m_magazineSpecLabel->getContentSize().height));
	m_magazineSpecLabel->setScale(0.3f);
	this->addChild(m_magazineSpecLabel);
	
	//init weapons
	initAllWeapon();

	//init name
	auto nameLabel = Label::create(m_playerName,"fonts/HashedBrowns-WyJgn.ttf",15);
	this->addChild(nameLabel);
	nameLabel->setTag(100);
	nameLabel->setColor(Color3B(255, 255, 255));
	nameLabel->setPosition(Vec2(this->getContentSize().width / 2, 20));

	this->setName(m_playerName);

	return true;
}

bool Player::loadGraphs()
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
	case actions::stand:
		anime = Animation::createWithSpriteFrames(m_standBackAnime, 0.5f / 2);
		break;
	case actions::leftSlide:
		anime = Animation::createWithSpriteFrames(m_leftSlideAnime, 0.5f / 2);
		break;
	case actions::rightSlide:
		anime = Animation::createWithSpriteFrames(m_leftSlideAnime, 0.5f / 2);
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
	double offsetX = this->getCurrentSpeed()+1.0f;
	double offsetY = this->getCurrentSpeed()+1.0f;
	if (isMe())
	{
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

	//update visual specs
	if (m_currentWeapon != nullptr)
		if(m_currentWeapon->m_ammoInCurrentMagazine>=0)
		m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());
		else
			m_magazineSpecLabel->setString("inf/inf");

	//auto fire weapon
	swapWeapon(m_currentWeaponSlot);
	if (m_mouseButtonMap[EventMouse::MouseButton::BUTTON_LEFT] && m_currentWeapon->m_isAutoFire)
	{
		if (isMe())
		{
			if (Specs::getInstance()->isAimbotActivated())
			{
				auto allTargets = SpriteLayer::getInstance()->getAllTargets();
				_fireStartPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
				_fireTerminalPos = allTargets.front()->getParent()->convertToWorldSpace(allTargets.front()->getPosition());
			}
			else
			{
				_fireStartPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
				_fireTerminalPos = CrossHair::getInstance()->getCursorPos();
			}
		}
		_fireStartPos = BulletLayer::getInstance()->convertToNodeSpace(_fireStartPos);
		_fireTerminalPos = BulletLayer::getInstance()->convertToNodeSpace(_fireTerminalPos);
		m_currentWeapon->fire(_fireStartPos, _fireTerminalPos);
	}


	if (Specs::getInstance()->isSinglePlayer()||(!isMe()))
		return;

	neb::CJsonObject ojson = buildSyncData();
	ojson.Add("Tag", Specs::getInstance()->getPlayerName());


	if (Specs::getInstance()->isServer()) //send as server
	{
		SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());
	}
	else //send as client
	{
		SocketClient::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());
	}

}

void Player::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	m_keyMap[keycode] = true; //set pressed status to true
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_W:
		m_sprite->stopActionByTag(stand);
		runActionAnime(actions::forward);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		m_sprite->stopActionByTag(stand);
		runActionAnime(actions::back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_sprite->stopActionByTag(stand);
		runActionAnime(actions::left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_sprite->stopActionByTag(stand);
		runActionAnime(actions::right);
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		m_sprite->stopActionByTag(stand);
		if (m_keyMap[EventKeyboard::KeyCode::KEY_A])
			runActionAnime(actions::leftSlide);
		else
			runActionAnime(actions::rightSlide);
		break;
	case EventKeyboard::KeyCode::KEY_F:
		fastMeleeAttack();
		break;
	case EventKeyboard::KeyCode::KEY_1:
		m_currentWeaponSlot = 1;
		break;
	case EventKeyboard::KeyCode::KEY_2:
		m_currentWeaponSlot = 2;
		break;
	case EventKeyboard::KeyCode::KEY_3:
		m_currentWeaponSlot = 3;
		break;
	case EventKeyboard::KeyCode::KEY_4:
		m_currentWeaponSlot = 4;
		break;
	case EventKeyboard::KeyCode::KEY_5:
		m_currentWeaponSlot = 5;
		break;
	case EventKeyboard::KeyCode::KEY_6:
		m_currentWeaponSlot = 6;
		break;
	case EventKeyboard::KeyCode::KEY_7:
		m_currentWeaponSlot = 7;
		break;
	case EventKeyboard::KeyCode::KEY_R:
		m_currentWeapon->reload();
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
		m_sprite->stopActionByTag(actions::back);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		m_sprite->stopActionByTag(actions::left);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		m_sprite->stopActionByTag(actions::right);
		break;
	case EventKeyboard::KeyCode::KEY_SHIFT:
		m_sprite->stopActionByTag(actions::leftSlide);
		m_sprite->stopActionByTag(actions::rightSlide);
		break;
	default:
		break;
	}
	if (!(m_keyMap[EventKeyboard::KeyCode::KEY_W] || m_keyMap[EventKeyboard::KeyCode::KEY_S] || m_keyMap[EventKeyboard::KeyCode::KEY_A] || m_keyMap[EventKeyboard::KeyCode::KEY_D]))
		runActionAnime(stand);
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

void Player::setControlOffListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->removeEventListenersForTarget(this, false);
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
			if (Specs::getInstance()->isAimbotActivated())
			{
				auto allTargets = SpriteLayer::getInstance()->getAllTargets();
				_fireStartPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
				_fireTerminalPos = allTargets.front()->getParent()->convertToWorldSpace(allTargets.front()->getPosition());
			}
			else
			{
				_fireStartPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
				_fireTerminalPos = CrossHair::getInstance()->getCursorPos();
			}
		_fireStartPos = BulletLayer::getInstance()->convertToNodeSpace(_fireStartPos);
		_fireTerminalPos = BulletLayer::getInstance()->convertToNodeSpace(_fireTerminalPos);
		m_currentWeapon->fire(_fireStartPos, _fireTerminalPos);
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
	if (m_lastWeaponSlot == num)
		return;

	if (m_allWeaponsMap[num]->isLocked() && (!Specs::getInstance()->isAllWeaponActivated())&&isMe())
	{
		auto notification = Label::createWithTTF("This weapon isn't unlocked yet", "fonts/Notification Font.ttf", 10);
		notification->setColor(Color3B(255, 4, 56));
		notification->setPosition(Vec2(this->getParent()->getContentSize().width / 2, -2));
		this->getParent()->addChild(notification);
		auto fadein = FadeIn::create(0.25f);
		auto fadeout = FadeOut::create(0.25f);
		auto fadeSequence = Sequence::create(fadein, fadeout, NULL);
		auto repeat = Repeat::create(fadeSequence, 2);
		auto sequence = Sequence::create(repeat, CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, notification)), NULL);
		notification->runAction(sequence);
		return;
	}

	//Update sprite
	if (m_currentWeapon != nullptr)
	{
		m_currentWeapon->setVisible(false);
		m_currentWeapon->pauseControlListen();
	}
	m_currentWeapon = m_allWeaponsMap[num];
	m_currentWeapon->setVisible(true);
	m_currentWeapon->resumeControlListen();
	m_lastWeaponSlot = m_currentWeaponSlot;
	m_currentWeaponSlot = num;

	m_magazineSpecLabel->setString(Value(m_currentWeapon->m_ammoInCurrentMagazine).asString() + "/" + Value(m_currentWeapon->m_backupAmmo).asString());

}

void Player::unlockWeapon(int num)
{
	m_allWeaponsMap[num]->unlock();
}

Weapon* Player::getCurrentWeapon()
{
	return m_currentWeapon;
}

std::map<int, Weapon*> Player::getAllWeaponMap()
{
	return m_allWeaponsMap;
}

void Player::addWeapon(Weapon* weapon)
{
	m_allWeaponsMap.insert(std::make_pair(weapon->getWeaponType(), weapon));
	m_sprite->addChild(weapon);
	weapon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	weapon->setPosition(Vec2(0, m_sprite->getContentSize().height / 3));
	weapon->setVisible(false);
	weapon->setControlOnListen();
}

void Player::initAllWeapon()
{
	m_lastWeaponSlot = 1;

	auto wbigKnife = Weapon::createWeapon();
	wbigKnife->setWeaponType(weaponType::bigKnife);
	addWeapon(wbigKnife);

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

	wbigKnife->unlock();
	Player::swapWeapon(0);
}

void Player::fastMeleeAttack()
{
	swapWeapon(0);
	_fireStartPos = m_currentWeapon->getParent()->convertToWorldSpace(m_currentWeapon->getPosition());
	_fireTerminalPos = CrossHair::getInstance()->getCursorPos();
	_fireStartPos = BulletLayer::getInstance()->convertToNodeSpace(_fireStartPos);
	_fireTerminalPos = BulletLayer::getInstance()->convertToNodeSpace(_fireTerminalPos);
	m_currentWeapon->fire(_fireStartPos, _fireTerminalPos);
}

neb::CJsonObject Player::buildSyncData()
{
	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::PlayerData);
	//keymap data
	ojson.AddEmptySubArray("KeyMap");
	ojson["KeyMap"].Add(m_keyMap[EventKeyboard::KeyCode::KEY_W]);
	ojson["KeyMap"].Add(m_keyMap[EventKeyboard::KeyCode::KEY_A]);
	ojson["KeyMap"].Add(m_keyMap[EventKeyboard::KeyCode::KEY_S]);
	ojson["KeyMap"].Add(m_keyMap[EventKeyboard::KeyCode::KEY_D]);

	//weapon data
	ojson.Add("Slot", m_currentWeaponSlot);

	//position data
	ojson.AddEmptySubArray("Position");
	ojson["Position"].Add(this->getPosition().x);
	ojson["Position"].Add(this->getPosition().y);

	//fire data
	ojson.Add("IsFire", m_mouseButtonMap[EventMouse::MouseButton::BUTTON_LEFT]);
	ojson.AddEmptySubArray("FireStart");
	ojson["FireStart"].Add(_fireStartPos.x);
	ojson["FireStart"].Add(_fireStartPos.y);
	ojson.AddEmptySubArray("FireEnd");
	ojson["FireEnd"].Add(_fireTerminalPos.x);
	ojson["FireEnd"].Add(_fireTerminalPos.y);

	return ojson;
}

void Player::updateWithSyncData(neb::CJsonObject ojson)
{
	////update pos
	//int boolValue = 0;
	//ojson["KeyMap"].Get(0,boolValue);
	//m_keyMap[EventKeyboard::KeyCode::KEY_W] = boolValue > 0;
	//ojson["KeyMap"].Get(1, boolValue);
	//m_keyMap[EventKeyboard::KeyCode::KEY_A] = boolValue > 0;
	//ojson["KeyMap"].Get(2, boolValue);
	//m_keyMap[EventKeyboard::KeyCode::KEY_S] = boolValue > 0;
	//ojson["KeyMap"].Get(3, boolValue);
	//m_keyMap[EventKeyboard::KeyCode::KEY_D] = boolValue > 0;

	//update pos
	double posx, posy;
	ojson["Position"].Get(0, posx);
	ojson["Position"].Get(1, posy);
	this->setPosition(Vec2(posx, posy));

	//update weapon
	ojson.Get("Slot", m_currentWeaponSlot);

	//fire
	int boolValue = 0;
	ojson.Get("IsFire", boolValue);
	m_mouseButtonMap[EventMouse::MouseButton::BUTTON_LEFT] = boolValue > 0;
	ojson["FireStart"].Get(0, _fireStartPos.x);
	ojson["FireStart"].Get(1, _fireStartPos.y);
	ojson["FireEnd"].Get(0, _fireTerminalPos.x);
	ojson["FireEnd"].Get(1, _fireTerminalPos.y);

	if (m_mouseButtonMap[EventMouse::MouseButton::BUTTON_LEFT])
	{
		m_currentWeapon->fire(_fireStartPos, _fireTerminalPos);
	}

}
