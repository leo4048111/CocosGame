#include "GameoverScene.h"
#include "MenuScene.h"
#include "Controls/Specs.h"

USING_NS_CC;

GameoverScene* GameoverScene::createGameoverScene()
{
	return GameoverScene::create();
}

bool GameoverScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto notificationLabel = Label::createWithTTF("You lose", "fonts/A Damn Mess.ttf", 50);
	auto notification = MenuItemLabel::create(notificationLabel);
	notification->setColor(Color3B(178, 34, 34));
	if (Specs::getInstance()->isWin())
	{
		notification->setString("You win");
		notification->setColor(Color3B(207, 167, 36));
	}
	auto backToMenuLabel = Label::createWithTTF("Back To Menu","fonts/HashedBrowns-WyJgn.ttf",20);
	auto backToMenu = MenuItemLabel::create(backToMenuLabel, CC_CALLBACK_1(GameoverScene::gameoverCallBack, this));
	backToMenu->setTag(0);
	backToMenu->setColor(Color3B(220, 220, 220));
	auto menu = Menu::create(notification, backToMenu, NULL);
	this->addChild(menu);
	menu->setPositionY(menu->getPositionY() - 40);
	menu->alignItemsVerticallyWithPadding(50);

	auto scoreLabel = Label::createWithTTF("Highest Score: "+Value(Specs::getInstance()->getScore()).asString(), "fonts/HashedBrowns-WyJgn.ttf", 20);
	scoreLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));
	this->addChild(scoreLabel);

	return true;
}

void GameoverScene::gameoverCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	switch (item->getTag())
	{
	case 0:
		backToMenuScene();
		break;

	default:
		break;
	}
}

void GameoverScene::backToMenuScene()
{
	auto scene = MenuScene::createMenuScene();
	auto transition = TransitionCrossFade::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
}