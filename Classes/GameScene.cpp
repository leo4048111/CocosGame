#include "GameScene.h"

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
}