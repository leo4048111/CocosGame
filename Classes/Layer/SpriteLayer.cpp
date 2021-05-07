#include "SpriteLayer.h"
#include "Algorithm/msws.h"

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
	//init main character
	auto mainCharacter = MainCharacter::createMainCharacter();
	this->addChild(mainCharacter, 20, "MainCharacter");
	int mapWidth = this->getContentSize().width;
	int mapHeight = this->getContentSize().height;
	mainCharacter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	mainCharacter->setPosition(Vec2(origin.x+visibleSize.width / 2,origin.y+ visibleSize.height / 3));
	mainCharacter->scheduleUpdate();

	this->setName("SpriteLayer");
	return true;
}

void SpriteLayer::update(float delta)
{
	//update target
	if (m_targets.size() < MIN_TARGETS_COUNT)
	{
		msws_srand();
		for (int c = m_targets.size(); c < MIN_TARGETS_COUNT; c++)
		{
			auto target = Target::createTarget();
			this->addChild(target, 20);
			target->setPosition(Vec2(msws()%((int)Director::getInstance()->getVisibleSize().width), msws() % ((int)Director::getInstance()->getVisibleSize().height))); 
			m_targets.pushBack(target);
			target->scheduleUpdate();
		}
	}
}