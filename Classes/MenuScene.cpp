#include "MenuScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* MenuScene::createMainScene()
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


	auto startGame = MenuItemFont::create("Start Game", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	auto audioControl = MenuItemFont::create("Audio Off",CC_CALLBACK_1(MenuScene::menuCallBack,this));
	auto quitGame = MenuItemFont::create("Quit", CC_CALLBACK_1(MenuScene::menuCallBack, this));
	startGame->setTag(0);
	audioControl->setTag(1);
	quitGame->setTag(2);
	auto menu = Menu::create(startGame,audioControl,quitGame,NULL);
	menu->alignItemsVerticallyWithPadding(30);
	this->addChild(menu,20);
	menu->setName("menu");

	auto backgroundImg = Sprite::create("background.png");
	backgroundImg->setPosition(origin + visibleSize / 2);
	this->addChild(backgroundImg,10);
}

enum menuItems
{
	sg,ac,qg
};

void MenuScene::menuCallBack(Ref* sender)
{
	MenuItemFont* item = dynamic_cast<MenuItemFont*>(sender);
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
	MenuItemFont* audioControl = dynamic_cast<MenuItemFont*>(this->getChildByName("menu")->getChildByTag(ac));
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