#include "UILayer.h"
#include <ctime>
USING_NS_CC;

UILayer* UILayer::createUILayer()
{
	return UILayer::create();
}

bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto m_visibleSize = Director::getInstance()->getVisibleSize();
	auto m_origin = Director::getInstance()->getVisibleOrigin();
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

	//init timer
	m_startTime = std::time(NULL);
	m_endTime = std::time(NULL);
	double runTime = static_cast<double>(m_endTime - m_startTime) / CLOCKS_PER_SEC;
	m_labelTimer = Label::create(Value(runTime).asString() + "s", "HeiTi", 10);
	m_labelTimer->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height));
	m_labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->addChild(m_labelTimer, 100,"Timer");

	//init crosshair
	auto crossHair = CrossHair::createCrossHair();
	this->addChild(crossHair, 30, "CrossHair");
	crossHair->setPosition(m_origin + m_visibleSize / 2);

	this->setName("UILayer");
	return true;
}

void UILayer::update(float delta)
{
	//update timer
	m_endTime = std::time(NULL);
	int runTime = static_cast<int>((m_endTime - m_startTime) * 1000 / CLOCKS_PER_SEC);
	m_labelTimer->setString(Value(runTime).asString() + "s");
}