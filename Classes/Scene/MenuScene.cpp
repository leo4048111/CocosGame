#include "MenuScene.h"
#include "GameScene.h"
#include "Controls/Specs.h"
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

	auto startGameLabel = Label::createWithTTF("Start Game", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto startGame = MenuItemLabel::create(startGameLabel, CC_CALLBACK_1(MenuScene::menuCallBack, this));
	auto audioControlLabel = Label::createWithTTF("Audio Off", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto audioControl = MenuItemLabel::create(audioControlLabel, CC_CALLBACK_1(MenuScene::menuCallBack,this));
	auto quitGameLabel = Label::createWithTTF("Quit", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto quitGame = MenuItemLabel::create(quitGameLabel, CC_CALLBACK_1(MenuScene::menuCallBack, this));
	startGame->setTag(0);
	startGame->setColor(Color3B(220, 220, 220));
	audioControl->setTag(1);
	audioControl->setColor(Color3B(220, 220, 220));
	quitGame->setTag(2);
	quitGame->setColor(Color3B(220, 220, 220));

	auto menu = Menu::create(startGame,audioControl,quitGame,NULL);
	menu->alignItemsVerticallyWithPadding(30);
	this->addChild(menu,20);
	menu->setName("menu");

	/*auto backgroundImg = Sprite::create("background.png");
	backgroundImg->setPosition(origin + visibleSize / 2);
	this->addChild(backgroundImg,10);*/
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
		startGame();
		break;
	case ac:
		setBackgroundMusic();
		break;
	case qg:
		exit(0);
	}
}

void MenuScene::startGame()
{
	auto gameScene = GameScene::createGameScene();
	auto transition = TransitionFlipX::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}

void MenuScene::setBackgroundMusic()
{
	MenuItemLabel* audioControl = dynamic_cast<MenuItemLabel*>(this->getChildByName("menu")->getChildByTag(ac));
	if (audioControl->getString()== "Audio Off")
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		audioControl->setString("Audio  On");
	}
	else
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		audioControl->setString("Audio Off");
	}
}