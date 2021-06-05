#include "PreparationScene.h"
#include "Controls/Specs.h"
#include "SetupScene.h"
#include "MenuScene.h"

USING_NS_CC;

SetupScene* SetupScene::createSetupScene()
{
	return SetupScene::create();
}

bool SetupScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	/*auto enterNameNotificationLabel = Label::createWithTTF("Enter your name below", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto enterNameNotification = MenuItemLabel::create(enterNameNotificationLabel);*/
	auto gtgLabel = Label::createWithTTF("Confirm", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto gtg = MenuItemLabel::create(gtgLabel, CC_CALLBACK_1(SetupScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(SetupScene::menuCallBack, this));

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

	m_word = Label::create("Tell me your name","fonts/HashedBrowns-WyJgn.ttf",20);
	m_word->setColor(Color3B(0, 0, 0));
	m_word->setPosition(Vec2(messageBubble->getContentSize().width / 2, messageBubble->getContentSize().height / 2 + 15));
	messageBubble->addChild(m_word);

	m_inputNameBox = TextFieldTTF::textFieldWithPlaceHolder("Enter your name", "Southampton-MRqp.ttf", 15);
	m_inputNameBox->setTextColor(Color4B(255, 255, 255, 100));
	this->addChild(m_inputNameBox);
	m_inputNameBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_inputNameBox->attachWithIME();

	auto fadeout = FadeOut::create(0.5f);
	auto fadein = FadeIn::create(0.5f);
	auto sequence = Sequence::create(fadeout, fadein, NULL);
	auto action = RepeatForever::create(sequence);
	m_inputNameBox->runAction(action);

	return true;
}

void SetupScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	std::string str = m_inputNameBox->getString();
	auto callFunc = CallFunc::create(CC_CALLBACK_0(SetupScene::goToPreparationScene, this));
	FiniteTimeAction* action = Sequence::create(DelayTime::create(1.5f), callFunc, NULL);
	switch (item->getTag())
	{
	case 0:
		if (str == "")
		{
			m_word->setString("You need to tell me \nyour name!");
			return;
		}
		Specs::getInstance()->setPlayerName(str);
		m_word->setString(str + "\nLet's see what you got.");
		this->runAction(action);
		break;
	case 1:
		backToMenuScene();
		break;
	default:
		break;
	}
}

void SetupScene::goToPreparationScene()
{
	PreparationScene* preparationScene = PreparationScene::create();
	auto transition = TransitionFlipX::create(0.5f, preparationScene);
	Director::getInstance()->replaceScene(transition);
}

void SetupScene::backToMenuScene()
{
	auto menuScene = MenuScene::createMenuScene();
	auto transition = TransitionFlipX::create(1.0f, menuScene);
	Director::getInstance()->replaceScene(transition);
}