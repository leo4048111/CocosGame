#include "GameScene.h"
#include "../Objects/MainCharacter.h"
#include "../Objects/CrossHair.h"

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
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();
	
	//init main character
	auto mainCharacter = MainCharacter::createMainCharacter();
	this->addChild(mainCharacter, 20);
	mainCharacter->setPosition(m_origin + m_visibleSize / 2);
	mainCharacter->scheduleUpdate();

	//init crosshair
	auto crossHair = CrossHair::createCrossHair();
	this->addChild(crossHair, 30);
	crossHair->setPosition(m_origin + m_visibleSize / 2);
	crossHair->setControlOnListen();




	auto testBg = Sprite::create("Map/mainMap.png");
	this->addChild(testBg,10);
	testBg->setPosition(m_origin + m_visibleSize / 2);
	testBg->setScale(0.3f);

	return true;
}
