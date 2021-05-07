#include "BulletLayer.h"

USING_NS_CC;

BulletLayer* BulletLayer::createBulletLayer()
{
	return BulletLayer::create();
}

bool BulletLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->setName("BulletLayer");
	return true;
}

void BulletLayer::cleanBullet(Node* sender)
{
	Sprite* bullet = dynamic_cast<Sprite*>(sender);
	if (bullet != nullptr)
	{
		bullet->stopAllActions();
		bullet->removeFromParentAndCleanup(1);
	}
	this->getParent()->getChildByName("biu")->removeFromParentAndCleanup(1);
}