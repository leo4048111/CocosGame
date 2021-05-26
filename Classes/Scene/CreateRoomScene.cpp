#include "CreateRoomScene.h"
#include "SetupAndGTGScene.h"
#include "PreparationScene.h"
#include "Controls/Specs.h"

USING_NS_CC;

CreateRoomScene* CreateRoomScene::createCreateRoomScene()
{
	return CreateRoomScene::create();
}

bool CreateRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto confirmLabel = Label::createWithTTF("Confirm", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto confirm = MenuItemLabel::create(confirmLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	confirm->setTag(0);
	confirm->setColor(Color3B(220, 220, 220));
	confirm->setScale(0.3f);

	back->setTag(1);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(confirm, back, NULL);
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

	m_word = Label::createWithTTF("So, how many capable men\n are you looking for", "fonts/HashedBrowns-WyJgn.ttf", 20);
	m_word->setColor(Color3B(0, 0, 0));
	m_word->setPosition(Vec2(messageBubble->getContentSize().width / 2, messageBubble->getContentSize().height / 2 + 15));
	messageBubble->addChild(m_word);

	m_maxPlayer = 2;
	m_maxPlayerDisplay = Label::createWithTTF("Max Player:" + Value(m_maxPlayer).asString(), "fonts/HashedBrowns-WyJgn.ttf", 20);
	this->addChild(m_maxPlayerDisplay);
	m_maxPlayerDisplay->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 15));

	auto addButtonLabel = Label::createWithTTF("+", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto addButton = MenuItemLabel::create(addButtonLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	addButton->setTag(2);
	auto minusButtonLabel = Label::createWithTTF("-", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto minusButton = MenuItemLabel::create(minusButtonLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	minusButton->setTag(3);
	auto menu2 = Menu::create(addButton, minusButton, NULL);
	this->addChild(menu2);
	menu2->alignItemsHorizontallyWithPadding(10);

	return true;
}

void CreateRoomScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(CreateRoomScene::goToSetupAndGTGScene, this));
	FiniteTimeAction* action = Sequence::create(DelayTime::create(1.5f), callFunc, NULL);

	switch (item->getTag())
	{
	case 0:
		Specs::getInstance()->setMaxPlayer(m_maxPlayer);
		m_word->setString(Value(m_maxPlayer).asString()+"\nSure");
		this->runAction(action);
		break;
	case 1:
		backToPreparationScene();
		break;
	case 2:
		addMaxPlayer();
		break;
	case 3:
		minusMaxPlayer();
		break;
	default:
		break;
	}
}

void CreateRoomScene::goToSetupAndGTGScene()
{
	SetupAndGTGScene* setupAndGTGScene = SetupAndGTGScene::create();
	auto transition = TransitionFlipX::create(0.5f, setupAndGTGScene);
	Director::getInstance()->replaceScene(transition);
}

void CreateRoomScene::backToPreparationScene()
{
	SetupAndGTGScene* setupAndGTGScene = SetupAndGTGScene::create();
	auto transition = TransitionFlipX::create(0.5f, setupAndGTGScene);
	Director::getInstance()->replaceScene(transition);
}

void CreateRoomScene::addMaxPlayer()
{
	if(m_maxPlayer<=7)
	m_maxPlayer++;

	m_maxPlayerDisplay->setString("Max Player:"+Value(m_maxPlayer).asString());
}

void CreateRoomScene::minusMaxPlayer()
{
	if (m_maxPlayer > 2)
		m_maxPlayer--;

	m_maxPlayerDisplay->setString("Max Player:"+Value(m_maxPlayer).asString());
}