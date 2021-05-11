#include "GameScene.h"

USING_NS_CC;

GameScene* GameScene::createGameScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	//init window
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	//init map
	TMXTiledMap* map = TMXTiledMap::create("map/desert.tmx");
	map->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(map, 0,"Map");
	map->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height / 2));

	//init sprite layer
	SpriteLayer* spriteLayer = SpriteLayer::createSpriteLayer();
	spriteLayer->scheduleUpdate();
	map->addChild(spriteLayer, 50,"SpriteLayer");

	//init bullet layer
	BulletLayer* bulletLayer = BulletLayer::createBulletLayer();
	bulletLayer->scheduleUpdate();
	map->addChild(bulletLayer, 50, "BulletLayer");

	//init UI layer
	UILayer* uiLayer = UILayer::createUILayer();
	uiLayer->scheduleUpdate();
	this->addChild(uiLayer, 100, "UILayer");

	this->setControlOnListen();
	this->scheduleUpdate();
	return true;
}

void GameScene::setControlOnListen()
{
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	dispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
	switch (keycode)
	{
	case EventKeyboard::KeyCode::KEY_ESCAPE:
		goToGameSettings();
		break;
	default:
		break;
	}
}

void GameScene::goToGameSettings()
{
	auto gameSettingsLayer = GameSettingsLayer::createGameSettingsLayer();
	this->addChild(gameSettingsLayer, 100);
}

void GameScene::update(float delta)
{
	//update camera 
	TMXTiledMap* map = dynamic_cast<TMXTiledMap*>(this->getChildByName("Map"));
	Vec2 mainCharacterPos = map->getChildByName("SpriteLayer")->getChildByName("MainCharacter")->getPosition();
	Vec2 mapPos = map->getContentSize() / 2;
	Vec2 offsetPos = mainCharacterPos - mapPos;
	map->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2-offsetPos.x, m_origin.y + m_visibleSize.height / 2-offsetPos.y));

	
}