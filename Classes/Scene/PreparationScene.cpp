#include "PreparationScene.h"
#include "SetupScene.h"
#include "CreateRoomScene.h"
#include "JoinRoomScene.h"
#include "GameScene.h"
#include "Controls/Specs.h"
#include "Scene/MenuScene.h"

USING_NS_CC;

PreparationScene* PreparationScene::createPreparationScene()
{
	return PreparationScene::create();
}

bool PreparationScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	Specs::getInstance()->refreshInstance();

	auto singlePlayerLabel = Label::createWithTTF("Single Player", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto singlePlayer = MenuItemLabel::create(singlePlayerLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	auto createRoomLabel = Label::createWithTTF("Create Room", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto createRoom = MenuItemLabel::create(createRoomLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	auto joinRoomLabel = Label::createWithTTF("Join Room", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto joinRoom = MenuItemLabel::create(joinRoomLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(PreparationScene::menuCallBack, this));
	singlePlayer->setTag(0);
	singlePlayer->setColor(Color3B(220, 220, 220));
	singlePlayer->setScale(0.3f);
	createRoom->setTag(1);
	createRoom->setColor(Color3B(220, 220, 220));
	createRoom->setScale(0.3f);
	joinRoom->setTag(2);
	joinRoom->setColor(Color3B(220, 220, 220));
	joinRoom->setScale(0.3f);
	back->setTag(3);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(singlePlayer, createRoom, joinRoom, back, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu, 20);
	menu->setName("menu");
	menu->setPositionY(menu->getPositionY() - 50);

	auto instructor = Sprite::create("objects/UI/ui_instructor.png");
	instructor->setPosition(Vec2(visibleSize.width / 4, visibleSize.height * 3 / 5));
	this->addChild(instructor);
	instructor->setScale(0.7f);

	auto messageBubble = Sprite::create("objects/UI/ui_messageBubble.png");
	instructor->addChild(messageBubble);
	messageBubble->setPosition(Vec2(instructor->getContentSize().width + 80, instructor->getContentSize().height));

	m_word = Label::create("So, which one do you fancy?", "fonts/HashedBrowns-WyJgn.ttf", 20);
	m_word->setColor(Color3B(0, 0, 0));
	m_word->setPosition(Vec2(messageBubble->getContentSize().width / 2, messageBubble->getContentSize().height / 2 + 15));
	messageBubble->addChild(m_word);

	return true;
}

void PreparationScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(PreparationScene::startGame, this));
	FiniteTimeAction* action = Sequence::create(DelayTime::create(1.5f), callFunc, NULL);
	auto callFunc2 = CallFunc::create(CC_CALLBACK_0(PreparationScene::goToCreateRoomScene, this));
	FiniteTimeAction* action2 = Sequence::create(DelayTime::create(1.5f), callFunc2, NULL);
	auto callFunc3 = CallFunc::create(CC_CALLBACK_0(PreparationScene::goToJoinRoomScene, this));
	FiniteTimeAction* action3 = Sequence::create(DelayTime::create(1.5f), callFunc3, NULL);
	switch (item->getTag())
	{
	case 0:
		Specs::getInstance()->setGamemodeAsSinglePlayer(true);
		Specs::getInstance()->setMaxPlayer(1);
		m_word->setString("Well well, a lone wolf..");
		this->runAction(action);
		break;
	case 1:
		Specs::getInstance()->setGamemodeAsSinglePlayer(false);
		m_word->setString("Create Room");
		this->runAction(action2);
		break;
	case 2:
		Specs::getInstance()->setGamemodeAsSinglePlayer(false);
		m_word->setString("Join Room");
		this->runAction(action3);
		break;
	case 3:
		backToSetupScene();
	}
}

void PreparationScene::backToSetupScene()
{
	SetupScene* setupScene = SetupScene::createSetupScene();
	auto transition = TransitionFlipX::create(0.5f, setupScene);
	Director::getInstance()->replaceScene(transition);
}


void PreparationScene::goToCreateRoomScene()
{
	CreateRoomScene* createRoomScene = CreateRoomScene::createCreateRoomScene();
	auto transition = TransitionFlipX::create(0.5f, createRoomScene);
	Director::getInstance()->replaceScene(transition);
}

void PreparationScene::startGame()
{
	auto gameScene = GameScene::createGameScene();
	auto transition = TransitionFlipX::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}

void PreparationScene::goToJoinRoomScene()
{
	auto joinRoomScene = JoinRoomScene::createJoinRoomScene();
	auto transition = TransitionFlipX::create(1.0f, joinRoomScene);
	Director::getInstance()->replaceScene(transition);
}