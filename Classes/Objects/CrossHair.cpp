#include "CrossHair.h"

USING_NS_CC;

CrossHair* CrossHair::createCrossHair()
{
	return CrossHair::create();
}

bool CrossHair::init()
{
	if (!Entity::init())
	{
		return false;
	}

	//Init sprite
	auto crossHair = Sprite::create("objects/crossHair/crosshair_normal.png");
	crossHair->setScale(0.2f);
	this->bindSprite(crossHair);

	this->setName("CrossHair");
	this->setControlOnListen();
	return true;
}

void CrossHair::setControlOnListen()
{
	auto* dispatcher = Director::getInstance()->getEventDispatcher();
	auto* mouseListener = EventListenerMouse::create();
	mouseListener->onMouseMove = CC_CALLBACK_1(CrossHair::onMouseMove, this);
	dispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void CrossHair::onMouseMove(Event* event)
{
	EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
	this->setPosition(Vec2(mouseEvent->getCursorX(), mouseEvent->getCursorY()));
}