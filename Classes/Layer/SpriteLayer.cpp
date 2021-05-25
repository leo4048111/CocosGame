#include "SpriteLayer.h"
#include "Algorithm/msws.h"
#include "UILayer.h"
#include "Controls/Specs.h"

USING_NS_CC;

SpriteLayer* SpriteLayer::_instance = NULL;

SpriteLayer* SpriteLayer::createSpriteLayer()
{
	if (_instance == NULL)
		_instance = SpriteLayer::create();
	return _instance;
}

bool SpriteLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//init target specs
	this->initTargetSpecs();

	//init collectable layer
	auto collectableLayer = CollectableLayer::getInstance();
	this->addChild(collectableLayer,20,"CollectableLayer");
	collectableLayer->scheduleUpdate();

	m_aiCount = 0;
	this->setName("SpriteLayer");
	return true;
}

void SpriteLayer::update(float delta)
{
	m_playerCount = m_players.size();
	m_targetCount = m_targets.size();
	int maxPlayerCount = Specs::getInstance()->getMaxPlayer();

	//update player
	if(maxPlayerCount == 1&& m_playerCount < maxPlayerCount)
	{
		addPlayer(false);
	}

	//update target
	if (m_targetCount < MIN_TARGETS_COUNT+Specs::getInstance()->getCurrentRound())
	{
		msws_srand();
		addTarget();
	}
}

void SpriteLayer::addPlayer(bool isAi)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vec2 dst = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3);

	//init player
	auto player = Player::createPlayer();
	this->addChild(player, 20, Specs::getInstance()->getPlayerName());
	int mapWidth = this->getContentSize().width;
	int mapHeight = this->getContentSize().height;
	player->setPosition(dst);
	player->setAiControl(isAi);
	if (!isAi)
	{
	/*	player->deployTo(dst);*/
		player->setReady();
		player->setControlOnListen();
	}
	else
	{
		m_aiCount++;
		player->deployTo(dst);
		player->setName("AI" +Value(m_aiCount).asString());
	}

	m_players.pushBack(player);
	player->scheduleUpdate();
}

void SpriteLayer::addTarget()
{
	auto target = Target::createTarget();
	target->showStaminaBar();
	this->addChild(target, 20);
	target->setPosition(Vec2(msws() % ((int)Director::getInstance()->getVisibleSize().width), msws() % ((int)Director::getInstance()->getVisibleSize().height)));
	m_targets.pushBack(target);
	target->scheduleUpdate();
}

Vector<Target*>* SpriteLayer::getAllTargets()
{
	return &m_targets;
}

void SpriteLayer::initTargetSpecs()
{
	//init target score map
	m_targetScoreMap.insert(std::make_pair(targetType::ghost, 10));
	m_targetScoreMap.insert(std::make_pair(targetType::jellyGhost, 20));
	m_targetScoreMap.insert(std::make_pair(targetType::sadGhost, 5));
	m_targetScoreMap.insert(std::make_pair(targetType::spirit, 30));

}

int SpriteLayer::getThisTargetScore(Target* target)
{
	return m_targetScoreMap[target->getTargetType()];
}

int SpriteLayer::getPlayerCount()
{
	return m_playerCount;
}

int SpriteLayer::getAiCount()
{
	return m_aiCount;
}