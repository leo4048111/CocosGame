#include "UILayer.h"
#include "../objects/MainCharacter.h"
#include <ctime>
#include "Controls/Specs.h"

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
	double runTime = static_cast<int>(Specs::getInstance()->getCurrentTime() - Specs::getInstance()->getStartTime())*1000 / CLOCKS_PER_SEC;
	m_labelTimer = Label::create(Value(runTime).asString() + "s", "HeiTi", 10);
	m_labelTimer->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height));
	m_labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->addChild(m_labelTimer, 100,"Timer");

	//init crosshair
	auto crossHair = CrossHair::createCrossHair();
	this->addChild(crossHair, 30, "CrossHair");
	crossHair->setPosition(m_origin + m_visibleSize / 2);

	//init Score Board
	
	m_labelScoreBoard = Label::createWithSystemFont("Score:0", "HeiTi", 10);
	this->addChild(m_labelScoreBoard,30,"ScoreBoard");
	m_labelScoreBoard->setPosition(Vec2(m_labelScoreBoard->getContentSize().width/2, m_visibleSize.height- m_labelScoreBoard->getContentSize().height));

	//init current mainCharacter spec labels
	m_currentSpeedLabel = Label::createWithTTF("Speed:0", "fonts/RetroNewVersion-v6Jy.ttf", 15);
	m_currentSpeedLabel->setPosition(Vec2(m_visibleSize.width / 4, m_currentSpeedLabel->getContentSize().height));
	this->addChild(m_currentSpeedLabel);

	m_currentResistanceLabel = Label::createWithTTF("Resistance:0", "fonts/RetroNewVersion-v6Jy.ttf", 15);
	m_currentResistanceLabel->setPosition(Vec2(m_visibleSize.width*3 / 4, m_currentSpeedLabel->getContentSize().height));
	this->addChild(m_currentResistanceLabel);

	this->setName("UILayer");
	return true;
}

void UILayer::update(float delta)
{
	auto m_visibleSize = Director::getInstance()->getVisibleSize();
	auto m_origin = Director::getInstance()->getVisibleOrigin();

	//update timer
	double runTime = static_cast<double>(Specs::getInstance()->getCurrentTime() - Specs::getInstance()->getStartTime())*1000 / CLOCKS_PER_SEC;
	m_labelTimer->setString(Value(runTime).asString() + "s");

	//update score board
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("Map")->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	m_labelScoreBoard->setString("Score:" + Value(Specs::getInstance()->getScore()).asString());
	if (Specs::getInstance()->getScore() / SCORE_PER_ROUND >= Specs::getInstance()->getCurrentRound())
	{
		Specs::getInstance()->goToNextRound();
		//update specs on enter
		auto currentRoundDisplay = Label::createWithTTF("Round " + Value(Specs::getInstance()->getCurrentRound()).asString(), "fonts/ElecstromRegular-w1y4P.ttf", 40);
		currentRoundDisplay->setColor(Color3B(248,248,255));
		this->addChild(currentRoundDisplay, 100, "CurrentRoundDisplay");
		currentRoundDisplay->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height * 3 / 4));

		//run action, fade in, display once and fade out
		auto fadeIn = FadeIn::create(4.0f);
		auto actionFadeIn = Repeat::create(fadeIn, 1);
		auto fadeOut = FadeOut::create(1.0f);
		auto actionFadeOut = Repeat::create(fadeOut, 1);
		auto sequence = Sequence::createWithTwoActions(actionFadeIn, actionFadeOut);

		currentRoundDisplay->runAction(sequence);
	}

	if(Specs::getInstance()->getCurrentRound() == 20)
		Specs::getInstance()->setWinOrLose(true);

	//update mainCharacter specs
	m_currentResistanceLabel->setString("Resistance:" + Value((int)mainCharacter->getCurrentResistance()).asString());
	m_currentSpeedLabel->setString("Speed:" + Value((int)mainCharacter->getCurrentSpeed()*1000).asString());

	//gameover
	if (Specs::getInstance()->isEnd())
	{
		goToGameoverScene();
	}
}


void UILayer::goToGameoverScene()
{
	GameoverScene* gameoverScene = GameoverScene::createGameoverScene();
	auto transition = TransitionFadeDown::create(1.0f, gameoverScene);
	Director::getInstance()->replaceScene(transition);
}
