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
	if (!loadAnimes())
	{
		return false;
	}
	Sprite* mainCharacter=Sprite::createWithSpriteFrameName("character_maleAdventurer_backwalk0");
	mainCharacter->setScale(0.1f);
	bindSprite(mainCharacter);

	this->setControlOnListen();
	
}

bool MainCharacter::loadAnimes()
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
	double offsetX = 1.3f;
	double offsetY = 1.3f;
	if (keyMap[EventKeyboard::KeyCode::KEY_SHIFT])
	{
		offsetX += 0.4;
		offsetY += 0.4;
	}
	if (keyMap[EventKeyboard::KeyCode::KEY_W])
	{
		m_sprite->setPosition(m_sprite->getPosition() + Vec2(0, offsetY));
	}
	if (keyMap[EventKeyboard::KeyCode::KEY_S])
	{
		m_sprite->setPosition(m_sprite->getPosition() - Vec2(0, offsetY));
	}
	if (keyMap[EventKeyboard::KeyCode::KEY_A])
	{
		m_sprite->setPosition(m_sprite->getPosition() - Vec2(offsetX, 0));
	}
	if (keyMap[EventKeyboard::KeyCode::KEY_D])
	{
		m_sprite->setPosition(m_sprite->getPosition() + Vec2(offsetX, 0));
	}
}

void MainCharacter::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	keyMap[keycode] = true; //set pressed status to true
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
	default:
		break;
	}
}

void MainCharacter::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	keyMap[keycode] = false; //set pressed status to false
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
	keyListener->onKeyPressed = CC_CALLBACK_2(MainCharacter::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(MainCharacter::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}