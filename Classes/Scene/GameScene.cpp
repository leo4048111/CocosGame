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
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();
	
	//init timer
	m_startTime = std::time(NULL);
	m_endTime = std::time(NULL);
	double runTime = static_cast<double>(m_endTime - m_startTime) / CLOCKS_PER_SEC;
	m_labelTimer = Label::create(Value(runTime).asString()+"s", "HeiTi", 10);
	m_labelTimer->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height));
	m_labelTimer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	this->addChild(m_labelTimer);

	//init main character
	auto mainCharacter = MainCharacter::createMainCharacter();
	this->addChild(mainCharacter, 20);
	mainCharacter->setPosition(m_origin + m_visibleSize / 2);
	mainCharacter->scheduleUpdate();

	//init crosshair
	auto crossHair = CrossHair::createCrossHair();
	this->addChild(crossHair, 30);
	crossHair->setPosition(m_origin + m_visibleSize / 2);

	////init background
	//auto testBg = Sprite::create("Map/mainMap.png");
	//this->addChild(testBg,10);
	//testBg->setPosition(m_origin + m_visibleSize / 2);
	//testBg->setScale(0.3f);

	//init targets
	for (int c = 0; c < MIN_TARGETS_COUNT; c++)
	{
		auto target = Target::createTarget();
		this->addChild(target, 20);
		target->setPosition(m_origin + m_visibleSize / 2);  //should be init with a random parser
		m_targets.pushBack(target);
	}


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
	//update target
	if (m_targets.size() < MIN_TARGETS_COUNT)
	{
		for (int c = m_targets.size(); c < MIN_TARGETS_COUNT; c++)
		{
			auto target = Target::createTarget();
			this->addChild(target, 20); 
			target->setPosition(m_origin + m_visibleSize / 2); //should be init with a random parser
			m_targets.pushBack(target);
		}
	}
	
	//update timer
	m_endTime = std::time(NULL);
	int runTime = static_cast<int>((m_endTime - m_startTime)*1000 / CLOCKS_PER_SEC);
	m_labelTimer->setString(Value(runTime).asString()+"s");

	//update hit status

}