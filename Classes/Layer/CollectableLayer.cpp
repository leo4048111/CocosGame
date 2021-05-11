#include "CollectableLayer.h"

USING_NS_CC;

CollectableLayer* CollectableLayer::createCollectableLayer()
{
	return CollectableLayer::create();
}

bool CollectableLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}


	return true;
}

void CollectableLayer::addCollectable(Collectable* collectable,float x,float y)
{
	this->addChild(collectable);
	collectable->setPosition(x, y);
	m_allCollectables.pushBack(collectable);
	collectable->scheduleUpdate();
}

void CollectableLayer::update(float delta)
{
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("MainCharacter"));
	Rect mainCharacterRect = Rect(mainCharacter->getBoundingBox().origin, mainCharacter->m_sprite->getContentSize());
	Vector<Collectable*> tmpDeleteCollectable;
	for (auto currentCollectable : m_allCollectables)
	{
		Rect collectableRect = currentCollectable->getBoundingBox();
		if (!currentCollectable->isStillValid())
		{
			currentCollectable->runDeadAction();
			tmpDeleteCollectable.pushBack(currentCollectable);
		}
		else if (mainCharacterRect.intersectsRect(collectableRect))
		{
			currentCollectable->useCollectable();
			tmpDeleteCollectable.pushBack(currentCollectable);
		}
	}

	for (auto currentCollectable : tmpDeleteCollectable)
	{
		m_allCollectables.eraseObject(currentCollectable);
		currentCollectable->removeFromParentAndCleanup(true);
	}
	tmpDeleteCollectable.clear();
}