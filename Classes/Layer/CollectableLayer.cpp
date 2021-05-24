#include "CollectableLayer.h"
#include "Controls/Specs.h"

USING_NS_CC;

CollectableLayer* CollectableLayer::_instance = NULL;

CollectableLayer* CollectableLayer::createCollectableLayer()
{
	if (_instance == NULL)
		_instance=CollectableLayer::create();
	return _instance;
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
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName(Specs::getInstance()->getPlayerName()));
	if (player == NULL)
		return;
	Rect playerRect = Rect(player->getBoundingBox().origin, player->m_sprite->getContentSize()/5);
	Vector<Collectable*> tmpDeleteCollectable;
	for (auto currentCollectable : m_allCollectables)
	{
		Rect collectableRect = currentCollectable->getBoundingBox();
		if (!currentCollectable->isStillValid())
		{
			currentCollectable->runDeadAction();
			tmpDeleteCollectable.pushBack(currentCollectable);
		}
		else if (playerRect.intersectsRect(collectableRect))
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