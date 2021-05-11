#include "BulletLayer.h"
#include "SpriteLayer.h"
#include "objects/CrossHair.h"
#include "UILayer.h"

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
		m_allBullets.eraseObject(bullet);
		bullet->removeFromParentAndCleanup(1);
	}
	
}

void BulletLayer::update(float delta)
{
	SpriteLayer* spriteLayer =dynamic_cast<SpriteLayer*>(this->getParent()->getChildByName("SpriteLayer"));
	Vector<Target*>* allTargets = spriteLayer->getAllTargets();
	UILayer* uiLayer = dynamic_cast<UILayer*>(this->getParent()->getParent()->getChildByName("UILayer"));
	Vector<Sprite*> tmpEraseBullet;
	Vector<Target*> tmpEraseTarget;
	for (auto currentBullet : m_allBullets)
	{
		for (auto currentTarget : *allTargets)
		{
			targetType currentTargetType = currentTarget->getTargetType();
			/*Rect bulletRect = Rect(currentBullet->getParent()->convertToWorldSpaceAR(currentBullet->getBoundingBox().origin), currentBullet->getBoundingBox().size);
			Rect targetRect = Rect(currentTarget->getParent()->convertToWorldSpaceAR(currentTarget->getBoundingBox().origin), currentTarget->getBoundingBox().size);*/
			/*MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
			Rect mainCharactere = Rect(mainCharacter->getParent()->convertToNodeSpace(mainCharacter->m_sprite->getBoundingBox().origin), mainCharacter->m_sprite->getBoundingBox().size);*/
			Rect bulletRect = currentBullet->getBoundingBox();
			Rect targetRect = Rect(currentTarget->getBoundingBox().origin,currentTarget->m_sprite->getContentSize()/5);
			if (bulletRect.intersectsRect(targetRect)) //Collision Detection
			{
				OutputDebugString("HIT TARGET\n");
				if(!currentTarget->receiveDamage(10))
				{
					uiLayer->addScore(spriteLayer->getThisTargetScore(currentTarget));
					tmpEraseTarget.pushBack(currentTarget);
				}
				tmpEraseBullet.pushBack(currentBullet);
				break;
			}
		}
	}

	while (!tmpEraseBullet.empty())
	{
		auto tmpBullet = tmpEraseBullet.back();
		try
		{
			tmpBullet->stopAllActions();
			tmpEraseBullet.popBack();
			m_allBullets.eraseObject(tmpBullet);
			tmpBullet->removeFromParentAndCleanup(true);
		}
		catch (const std::exception& exp)
		{
			CCLOG("%s", exp.what());
		}
	}

	while (!tmpEraseTarget.empty())
	{
		auto tmpTarget = tmpEraseTarget.back();
		if (tmpTarget != nullptr)
		{
			tmpTarget->dropRandomCollectable();
			tmpEraseTarget.popBack();
			allTargets->eraseObject(tmpTarget);
			tmpTarget->runDeadAction();
		}
	}
}

void BulletLayer::addBullet()
{
	//create bullet
	auto bullet = Sprite::create("objects/ammo/ammo_rifle.png");
	this->addChild(bullet);
	m_allBullets.pushBack(bullet);

	//set bullet rotation
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	CrossHair* crossHair =dynamic_cast<CrossHair*>(this->getParent()->getParent()->getChildByName("UILayer")->getChildByName("CrossHair"));
	Vec2 mousePosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 weaponPosVec = mainCharacter->getPosition();
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	bullet->setRotation(degree);
	bullet->setScale(0.2f);
	bullet->setPosition(mainCharacter->getPosition());

	//DEBUG
	std::string str1 = "ch:" + Value(mainCharacter->getPosition().x).asString() + "," + Value(mainCharacter->getPosition().y).asString() + "\n";
	OutputDebugString(str1.c_str());
	std::string str2 = "bullet:" + Value(bullet->getPosition().x).asString() + "," + Value(bullet->getPosition().y).asString() + "\n";
	OutputDebugString(str2.c_str());
	std::string str4 = "mouse:" + Value(mousePosVec.x).asString() + "," + Value(mousePosVec.y).asString() + "\n";
	OutputDebugString(str4.c_str());
	std::string str5 = "dst:" + Value(dst.x).asString() + "," + Value(dst.y).asString() + "\n";
	OutputDebugString(str5.c_str());
	std::string str3 = mainCharacter->getName() + "\n";
	OutputDebugString(str3.c_str());
	SpriteLayer* spriteLayer = dynamic_cast<SpriteLayer*>(this->getParent()->getChildByName("SpriteLayer"));
	Vector<Target*>* allTargets = spriteLayer->getAllTargets();

	//if (!allTargets->back()->receiveDamage(10))
	//{
	//	allTargets->eraseObject(allTargets->back());
	//}
	//DEBUG END

	auto action = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x)), sin(atan2(dst.y, dst.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));

}