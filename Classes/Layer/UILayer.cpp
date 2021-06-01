#include <ctime>
#include "UILayer.h"
#include "Controls/AudioControlPanel.h"
#include "../objects/Player.h"
#include "Controls/Specs.h"

USING_NS_CC;

UILayer* UILayer::_instance = NULL;

UILayer* UILayer::createUILayer()
{
	if (_instance == NULL)
		_instance = UILayer::create();
	return _instance;
}

bool UILayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//init timer
	int runTime = static_cast<int>(Specs::getInstance()->getCurrentTime() - Specs::getInstance()->getStartTime())*1000 / CLOCKS_PER_SEC;
	m_labelTimer = Label::createWithTTF("Survived for: "+Value(runTime).asString() + "s", "fonts/Retrofunkscriptpersonaluse-v6XO.otf", 15);
	m_labelTimer->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height));
	m_labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	m_labelTimer->setTextColor(Color4B(205, 92, 92, 200));
	this->addChild(m_labelTimer, 100,"Timer");

	//init crosshair
	auto crossHair = CrossHair::getInstance();
	this->addChild(crossHair, 30, "CrossHair");
	crossHair->setPosition(origin + visibleSize / 2);

	//init Score Board
	m_labelScoreBoard = Label::createWithTTF("Score:0", "fonts/RetroNewVersion-v6Jy.ttf", 15);
	this->addChild(m_labelScoreBoard,30,"ScoreBoard");
	m_labelScoreBoard->setColor(Color3B(255, 215, 0));
	m_labelScoreBoard->setPosition(Vec2(m_labelScoreBoard->getContentSize().width/2+20, visibleSize.height- m_labelScoreBoard->getContentSize().height));

	//init current mainCharacter spec labels
	m_currentSpeedLabel = Label::createWithTTF("Speed:0", "fonts/Retrofunkscriptpersonaluse-v6XO.otf", 15);
	m_currentSpeedLabel->setPosition(Vec2(visibleSize.width / 4, m_currentSpeedLabel->getContentSize().height));
	m_currentSpeedLabel->enableBold();
	m_currentSpeedLabel->setColor(Color3B(30, 144, 255));
	this->addChild(m_currentSpeedLabel);

	m_currentResistanceLabel = Label::createWithTTF("Resistance:0", "fonts/Retrofunkscriptpersonaluse-v6XO.otf", 15);
	m_currentResistanceLabel->setPosition(Vec2(visibleSize.width*3 / 4, m_currentSpeedLabel->getContentSize().height));
	m_currentResistanceLabel->enableBold();
	m_currentResistanceLabel->setColor(Color3B(178,34,34));
	this->addChild(m_currentResistanceLabel);

	//init weapon spec label
	m_weaponLabel = Label::createWithTTF("Current Weapon Slots:","fonts/HashedBrowns-WyJgn.ttf",15);
	this->addChild(m_weaponLabel);
	m_weaponLabel->setPosition(Vec2(visibleSize.width - m_weaponLabel->getContentSize().width+25, visibleSize.height * 5 / 6));
	m_weaponLabel->setColor(Color3B(210, 105, 30));
	for (int c = 0; c < weaponType::totalWeapons; c++)
	{
		auto slot = Label::createWithTTF(Value(c).asString(), "fonts/HashedBrowns-WyJgn.ttf", 15);
		slot->setPosition(Vec2(m_weaponLabel->getPosition().x, m_weaponLabel->getPosition().y - 15 * (c+1)));
		this->addChild(slot);
		m_weaponSlots.push_back(slot);
		slot->setColor(Color3B(216, 191, 216));

		auto unavailable = Sprite::create("objects/UI/ui_unavailable.png");
		unavailable->setScale(0.1f);
		slot->addChild(unavailable);
		unavailable->setPosition(15, slot->getContentSize().height/2);
		unavailable->setTag(1);
	}
	m_pointerArrow = Sprite::create("objects/UI/ui_pointerArrow.png");
	m_pointerArrow->setScale(0.1f);
	this->addChild(m_pointerArrow);

	//init chat system
	Chatbox* chatbox = Chatbox::getInstance();
	this->addChild(chatbox);
	chatbox->setPosition(Vec2(origin.x+100, origin.y + visibleSize.height * 1 / 5));
	chatbox->setControlOnListen();

	//init player status window

	m_statusWindowBg = Sprite::create("objects/UI/ui_playerStatusBg.png");
	m_statusWindowBg->setScale(0.2f);
	m_statusWindowBg->setPosition(Vec2(visibleSize.width/2,origin.y+20));
	this->addChild(m_statusWindowBg);
	m_playerHp = ProgressTimer::create(Sprite::create("objects/UI/ui_playerStatusHp.png"));
	m_playerHp->setType(ProgressTimer::Type::BAR);
	m_playerHp->setMidpoint(Vec2(0, 1.0f));
	m_playerHp->setPosition(Vec2(m_statusWindowBg->getContentSize().width/5-5, 150));
	m_playerHp->setBarChangeRate(Vec2(1.0f, 0));
	m_playerHp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_playerHp->setPercentage(100);
	m_statusWindowBg->addChild(m_playerHp);

	m_playerSpeed = ProgressTimer::create(Sprite::create("objects/UI/ui_playerStatusSpeed.png"));
	m_playerSpeed->setType(ProgressTimer::Type::BAR);
	m_playerSpeed->setMidpoint(Vec2(0, 1.0f));
	m_playerSpeed->setPosition(Vec2(m_statusWindowBg->getContentSize().width / 5-5,30));
	m_playerSpeed->setBarChangeRate(Vec2(1.0f, 0));
	m_playerSpeed->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_playerSpeed->setPercentage(100);
	m_statusWindowBg->addChild(m_playerSpeed);

	m_playerArmor = ProgressTimer::create(Sprite::create("objects/UI/ui_playerStatusArmor.png"));
	m_playerArmor->setType(ProgressTimer::Type::BAR);
	m_playerArmor->setMidpoint(Vec2(0, 1.0f));
	m_playerArmor->setPosition(Vec2(m_statusWindowBg->getContentSize().width / 5-5, 90));
	m_playerArmor->setBarChangeRate(Vec2(1.0f,0));
	m_playerArmor->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	m_playerArmor->setPercentage(100);
	m_statusWindowBg->addChild(m_playerArmor);

	//init audio control
	AudioControlPanel* audioControlPanel = AudioControlPanel::createAudioControlPanel();
	this->addChild(audioControlPanel);
	audioControlPanel->setPosition(visibleSize.width - audioControlPanel->getContentSize().width - 150, visibleSize.height - audioControlPanel->getContentSize().height - 50);

	this->setName("UILayer");
	return true;
}

void UILayer::unlockSlot(int num)
{
	m_weaponSlots[num]->setColor(Color3B(255, 215, 0));
	m_weaponSlots[num]->enableBold();
	m_weaponSlots[num]->removeChildByTag(1);
}

void UILayer::pointToThisSlot(int num)
{
	m_pointerArrow->setPosition(Vec2(m_weaponSlots[num]->getPosition().x - 15, m_weaponSlots[num]->getPosition().y));
}

void UILayer::update(float delta)
{
	auto m_visibleSize = Director::getInstance()->getVisibleSize();
	auto m_origin = Director::getInstance()->getVisibleOrigin();

	//update timer
	int runTime = static_cast<double>(Specs::getInstance()->getCurrentTime() - Specs::getInstance()->getStartTime())*1000 / CLOCKS_PER_SEC;
	m_labelTimer->setString("Survived for: "+Value(runTime).asString() + "s");

	//update score board
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("Map")->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	if (player == NULL)
		return;
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

	//update main player specs
	m_currentResistanceLabel->setString("Resistance:" + Value((int)player->getCurrentResistance()).asString());
	m_currentSpeedLabel->setString("Speed:" + Value((int)player->getCurrentSpeed()*1000).asString());

	//update current weapon slot
	pointToThisSlot(player->getCurrentWeapon()->getWeaponType());

	//update player status window
	m_playerHp->setPercentage(player->getHealthPercentage());
	double test = player->getSpeedPercentage();
	m_playerSpeed->setPercentage(player->getSpeedPercentage());
	m_playerArmor->setPercentage(player->getCurrentResistance());

	//update game status
	if (Specs::getInstance()->getCurrentRound() == 20)
		Specs::getInstance()->setWinOrLose(true);
}

void UILayer::instructorGivesInstruction(std::string str)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto instructor = Sprite::create("objects/UI/ui_instructor.png");
	instructor->setPosition(Vec2(0, visibleSize.height * 3 / 5));
	this->addChild(instructor);
	instructor->setScale(0.5f);

	auto messageBubble = Sprite::create("objects/UI/ui_messageBubble.png");
	instructor->addChild(messageBubble);
	messageBubble->setPosition(Vec2(instructor->getContentSize().width + 80, instructor->getContentSize().height));

	auto word = Label::create(str, "fonts/HashedBrowns-WyJgn.ttf", 20);
	word->setColor(Color3B(0, 0, 0));
	word->setPosition(Vec2(messageBubble->getContentSize().width / 2, messageBubble->getContentSize().height / 2 + 15));
	messageBubble->addChild(word);

	auto moveto = MoveTo::create(0.5f, Vec2(visibleSize.width / 6, instructor->getPosition().y));
	auto fadeout = FadeOut::create(1.0f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, instructor));
	auto sequence = Sequence::create(moveto, DelayTime::create(0.5f), fadeout, callFunc, NULL);

	instructor->runAction(sequence);
}