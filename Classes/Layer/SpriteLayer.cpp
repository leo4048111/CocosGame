#include "SpriteLayer.h"
#include "Algorithm/msws.h"
#include "UILayer.h"

USING_NS_CC;

SpriteLayer* SpriteLayer::createSpriteLayer()
{
	return SpriteLayer::create();
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
	auto mainCharacter = MainCharacter::createMainCharacter();
	this->addChild(mainCharacter, 20, "MainCharacter");
	int mapWidth = this->getContentSize().width;
	int mapHeight = this->getContentSize().height;
	mainCharacter->setPosition(Vec2(origin.x+visibleSize.width / 2,origin.y+ visibleSize.height / 3));
	mainCharacter->scheduleUpdate();

	//init collectable layer
	auto collectableLayer = CollectableLayer::createCollectableLayer();
	this->addChild(collectableLayer,20,"CollectableLayer");
	collectableLayer->scheduleUpdate();

	this->setName("SpriteLayer");
	return true;
}

void SpriteLayer::update(float delta)
{
	int currentTargetCount = m_targets.size();
	UILayer* uiLayer=dynamic_cast<UILayer*>(this->getParent()->getParent()->getChildByName("UILayer"));
	//update target
	if (currentTargetCount < MIN_TARGETS_COUNT+uiLayer->getCurrentRound())
	{
		msws_srand();
		for (int c = m_targets.size(); c < MIN_TARGETS_COUNT + uiLayer->getCurrentRound(); c++)
		{
			auto target = Target::createTarget();
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