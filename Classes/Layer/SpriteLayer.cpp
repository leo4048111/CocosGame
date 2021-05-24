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

	//init main character
	auto mainCharacter = Player::createMainCharacter();
	this->addChild(mainCharacter, 20, Specs::getInstance()->getPlayerName());
	int mapWidth = this->getContentSize().width;
	int mapHeight = this->getContentSize().height;
	mainCharacter->setPosition(Vec2(origin.x+visibleSize.width / 2,origin.y+ visibleSize.height / 3));
	mainCharacter->scheduleUpdate();

	//init collectable layer
	auto collectableLayer = CollectableLayer::getInstance();
	this->addChild(collectableLayer,20,"CollectableLayer");
	collectableLayer->scheduleUpdate();

	this->setName("SpriteLayer");
	return true;
}

void SpriteLayer::update(float delta)
{
	int currentTargetCount = m_targets.size();
	UILayer* uiLayer=UILayer::getInstance();
	//update target
	if (currentTargetCount < MIN_TARGETS_COUNT+Specs::getInstance()->getCurrentRound())
	{
		msws_srand();
		for (int c = m_targets.size(); c < MIN_TARGETS_COUNT + Specs::getInstance()->getCurrentRound(); c++)
		{
			auto target = Target::createTarget();
			target->showStaminaBar();
			this->addChild(target, 20);
			target->setPosition(Vec2(msws()%((int)Director::getInstance()->getVisibleSize().width), msws() % ((int)Director::getInstance()->getVisibleSize().height))); 
			m_targets.pushBack(target);
			target->scheduleUpdate();
		}
	}
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