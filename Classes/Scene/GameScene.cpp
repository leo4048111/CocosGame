#include "GameScene.h"
#include "../Objects/MainCharacter.h"

USING_NS_CC;

Scene* GameScene::createGameScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto mainCharacter = MainCharacter::createMainCharacter();
	this->addChild(mainCharacter);
	mainCharacter->setPosition(origin+visibleSize / 2);
	mainCharacter->scheduleUpdate();
	return true;
}