#include "UILayer.h"
#include "../objects/MainCharacter.h"
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

	//init Score Board
	m_currentScore = 0;
	m_labelScoreBoard = Label::createWithSystemFont("Score:0", "HeiTi", 10);
	this->addChild(m_labelScoreBoard,30,"ScoreBoard");
	m_labelScoreBoard->setPosition(Vec2(m_labelScoreBoard->getContentSize().width/2, m_visibleSize.height- m_labelScoreBoard->getContentSize().height));

	//init round specs
	m_currentRound = 1;
	m_lastRound = 0;

	this->setName("UILayer");
	return true;
}

void UILayer::update(float delta)
{
	auto m_visibleSize = Director::getInstance()->getVisibleSize();
	auto m_origin = Director::getInstance()->getVisibleOrigin();

	//update timer
	m_endTime = std::time(NULL);
	int runTime = static_cast<int>((m_endTime - m_startTime) * 1000 / CLOCKS_PER_SEC);
	m_labelTimer->setString(Value(runTime).asString() + "s");

	//update score board
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("Map")->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	m_labelScoreBoard->setString("Score:" + Value(this->getScore()).asString());

	//update round count
	if (m_currentRound != m_lastRound)
	{
		//update specs on enter
		auto currentRoundDisplay = Label::createWithSystemFont("Round " + Value(m_currentRound).asString(), "HeiTi",20);
		this->addChild(currentRoundDisplay, 100, "CurrentRoundDisplay");
		currentRoundDisplay->setPosition(Vec2(m_origin.x + m_visibleSize.width/2,m_origin.y+m_visibleSize.height*3/4));

		//run action, fade in, display once and fade out
		auto fadeIn = FadeIn::create(4.0f);
		auto actionFadeIn = Repeat::create(fadeIn, 1);
		auto fadeOut = FadeOut::create(1.0f);
		auto actionFadeOut = Repeat::create(fadeOut, 1);
		auto sequence = Sequence::createWithTwoActions(actionFadeIn, actionFadeOut);

		currentRoundDisplay->runAction(sequence);
		m_lastRound = m_currentRound;
	}
}

void UILayer::addScore(int score)
{
	m_currentScore += score;
}

int UILayer::getScore()
{
	return m_currentScore;
}