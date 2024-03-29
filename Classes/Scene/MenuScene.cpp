#include "MenuScene.h"
#include "SetupScene.h"
#include "Controls/Specs.h"
#include "Controls/AudioControlPanel.h"
#include "Network/SocketClient.h"
#include "Network/SocketServer.h"

USING_NS_CC;

MenuScene* MenuScene::createMenuScene()
{
	return MenuScene::create();
}

bool MenuScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	Specs::getInstance()->refreshInstance();
	SocketServer::getInstance()->destroyInstance();
	SocketClient::getInstance()->destroy();

	auto startGameLabel = Label::createWithTTF("Press To Start", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto startGame = MenuItemLabel::create(startGameLabel, CC_CALLBACK_1(MenuScene::menuCallBack, this));
	auto audioControlLabel = Label::createWithTTF("Field Trip Mode On", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto audioControl = MenuItemLabel::create(audioControlLabel, CC_CALLBACK_1(MenuScene::menuCallBack,this));
	auto quitGameLabel = Label::createWithTTF("Quit", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto quitGame = MenuItemLabel::create(quitGameLabel, CC_CALLBACK_1(MenuScene::menuCallBack, this));
	startGame->setTag(0);
	startGame->setColor(Color3B(220, 220, 220));
	startGame->setScale(0.3f);
	audioControl->setTag(1);
	audioControl->setColor(Color3B(220, 220, 220));
	audioControl->setScale(0.3f);
	quitGame->setTag(2);
	quitGame->setColor(Color3B(220, 220, 220));
	quitGame->setScale(0.3f);

	auto fadeout = FadeOut::create(0.5f);
	auto fadein = FadeIn::create(0.5f);
	auto sequence = Sequence::create(fadeout, fadein, NULL);
	auto action = RepeatForever::create(sequence);
	startGame->runAction(action);

	auto menu = Menu::create(startGame,audioControl,quitGame,NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu,20);
	menu->setName("menu");
	menu->setPositionY(menu->getPositionY() - 50);

	auto logo = Label::createWithTTF("POBG", "fonts/Southampton-MRqp.ttf", 80);
	this->addChild(logo);
	logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 5 / 7));
	logo->setColor(Color3B(218, 165, 32));
	logo->enableGlow(Color4B(255, 215, 0, 50));
	logo->enableOutline(Color4B(0,0,0,10));
	logo->enableBold();
	logo->enableShadow(Color4B(0, 0, 0, 10));
	
	auto info = Label::createWithTTF("made by 2050250", "fonts/Notification Font.ttf", 15);
	info->enableBold();
	this->addChild((info));
	info->setPosition(Vec2(logo->getPosition().x + logo->getContentSize().width-20, logo->getPosition().y-logo->getContentSize().height/2));
	info->setColor(Color3B(255, 255, 255));

	AudioControlPanel* audioControlPanel = AudioControlPanel::createAudioControlPanel();
	this->addChild(audioControlPanel);
	audioControlPanel->setPosition(visibleSize.width- audioControlPanel->getContentSize().width-150,visibleSize.height- audioControlPanel->getContentSize().height-50);
	return true;
}

enum menuItems
{
	sg,ac,qg
};

void MenuScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	switch (item->getTag())
	{
	case sg:
		goToSetupScene();
		break;
	case ac:
		setMode();
		break;
	case qg:
		exit(0);
	}
}

void MenuScene::setMode()
{
	MenuItemLabel* audioControl = dynamic_cast<MenuItemLabel*>(this->getChildByName("menu")->getChildByTag(ac));
	if (audioControl->getString() == "Field Trip Mode On")
	{
		Specs::getInstance()->toggleFieldTrip(true);
		audioControl->setString("Field Trip Mode Off");
	}
	else
	{
		Specs::getInstance()->toggleFieldTrip(false);
		audioControl->setString("Field Trip Mode On");
	}
}

void MenuScene::goToSetupScene()
{
	SetupScene* setupScene = SetupScene::createSetupScene();
	auto transition = TransitionFlipX::create(0.5f, setupScene);
	Director::getInstance()->replaceScene(transition);
}