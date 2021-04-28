#include "GameSettingsLayer.h"
#include "../Scene/GameScene.h"
#include "../Scene/MenuScene.h"

USING_NS_CC;

GameSettingsLayer* GameSettingsLayer::createGameSettingsLayer()
{
	return GameSettingsLayer::create();
}

bool GameSettingsLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto backToGame = MenuItemFont::create("Back To Game", CC_CALLBACK_1(GameSettingsLayer::gameSettingsCallBack, this));
	backToGame->setTag(0);
	auto backToMenu = MenuItemFont::create("Back To Menu", CC_CALLBACK_1(GameSettingsLayer::gameSettingsCallBack, this));
	backToMenu->setTag(1);
	auto gameSettingsMenu = Menu::create(backToGame, backToMenu, NULL);
	this->addChild(gameSettingsMenu);
	gameSettingsMenu->alignItemsVerticallyWithPadding(50);

	return true;
}

void GameSettingsLayer::gameSettingsCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	switch (item->getTag())
	{
	case 0:
		backToGameScene();
		break;
	case 1:
		backToMenuScene();
		break;
	default:
		break;
	}
}

void GameSettingsLayer::backToGameScene()
{
	this->removeFromParent();
}

void GameSettingsLayer::backToMenuScene()
{
	auto scene = MenuScene::createMenuScene();
	auto transition = TransitionFlipY::create(1.0f, scene);
	Director::getInstance()->replaceScene(transition);
}

