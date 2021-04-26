#include "EntityMovementControl.h"

USING_NS_CC;

EntityMovementControl* EntityMovementControl::createEntityControl()
{
	return EntityMovementControl::create();
}

bool EntityMovementControl::init()
{
	if (!Node::init())
	{
		return false;
	}

}

void EntityMovementControl::onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	keyMap[keycode] = true; //set pressed status to true
}

void EntityMovementControl::onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event)
{
	keyMap[keycode] = false; //set pressed status to false
}

void EntityMovementControl::setControlOnListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(EntityMovementControl::onKeyPressed, this);
	keyListener->onKeyReleased = CC_CALLBACK_2(EntityMovementControl::onKeyReleased, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
}