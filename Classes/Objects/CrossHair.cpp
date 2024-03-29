#include "CrossHair.h"

USING_NS_CC;

CrossHair* CrossHair::_instance = NULL;

CrossHair* CrossHair::createCrossHair()
{
	if (_instance == NULL)
		_instance = CrossHair::create();
	return _instance;
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

	//Init hit notification icon
	

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
	m_cursorPos.x = mouseEvent->getCursorX();
	m_cursorPos.y = mouseEvent->getCursorY();
	this->setPosition(m_cursorPos);

}

Vec2 CrossHair::getCursorPos()
{
	return m_cursorPos;
}

void CrossHair::showHitNotification() //This method is deprecated!! needs improvement
{
	auto hitNotification = Sprite::create("objects/crossHair/crosshair_hit.png");
	hitNotification->setScale(0.2f);
	this->addChild(hitNotification);
	auto fadeIn=FadeIn::create(0.2f);
	auto fadeOut = FadeOut::create(0.1f);
	auto scaleBy1 = ScaleBy::create(0.2f, 2.0f);
	auto scaleBy2 = ScaleBy::create(0.1f, 1 / 2.0f);
	auto spawn1 = Spawn::create(fadeIn, scaleBy1, NULL);
	auto spawn2 = Spawn::create(fadeOut, scaleBy2, NULL);
	auto sequence = Sequence::create(spawn1, spawn2, NULL);
	hitNotification->runAction(sequence);
}
