#include "PreparationScene.h"
#include "Controls/Specs.h"
#include "SetupAndGTGScene.h"
#include "GameScene.h"

USING_NS_CC;

SetupAndGTGScene* SetupAndGTGScene::createSetupAndGTGScene()
{
	return SetupAndGTGScene::create();
}

bool SetupAndGTGScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	Specs::getInstance()->refreshInstance();

	/*auto enterNameNotificationLabel = Label::createWithTTF("Enter your name below", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto enterNameNotification = MenuItemLabel::create(enterNameNotificationLabel);*/
	auto gtgLabel = Label::createWithTTF("Let's Roll", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto gtg = MenuItemLabel::create(gtgLabel, CC_CALLBACK_1(SetupAndGTGScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(SetupAndGTGScene::menuCallBack, this));

	//enterNameNotification->setColor(Color3B(220, 220, 220));
	//enterNameNotification->setScale(0.3f);
	gtg->setTag(0);
	gtg->setColor(Color3B(220, 220, 220));
	gtg->setScale(0.3f);
	back->setTag(1);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(gtg, back, NULL);
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
	messageBubble->setPosition(Vec2(instructor->getContentSize().width+80, instructor->getContentSize().height));

	m_word = Label::create("Tell me your name son","fonts/HashedBrowns-WyJgn.ttf",20);
	m_word->setColor(Color3B(0, 0, 0));
	m_word->setPosition(Vec2(messageBubble->getContentSize().width / 2, messageBubble->getContentSize().height / 2 + 15));
	messageBubble->addChild(m_word);

	m_inputNameBox = TextFieldTTF::textFieldWithPlaceHolder("Enter your name", "Southampton-MRqp.ttf", 15);
	m_inputNameBox->setTextColor(Color4B(255, 255, 255, 100));
	this->addChild(m_inputNameBox);
	m_inputNameBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_inputNameBox->attachWithIME();

	return true;
}

void SetupAndGTGScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	std::string str = m_inputNameBox->getString();
	switch (item->getTag())
	{
	case 0:
		if (str == "")
		{
			m_word->setString("You need to tell me \nyour name!");
			return;
		}
		Specs::getInstance()->setPlayerName(str);
		startGame();
		break;
	case 1:
		break;
	case 2:
		backToPreparationScene();
	}
}

void SetupAndGTGScene::backToPreparationScene()
{
	PreparationScene* menuScene = PreparationScene::create();
	auto transition = TransitionFlipX::create(0.5f, menuScene);
	Director::getInstance()->replaceScene(transition);
}

void SetupAndGTGScene::startGame()
{
	auto gameScene = GameScene::createGameScene();
	auto transition = TransitionFlipX::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}