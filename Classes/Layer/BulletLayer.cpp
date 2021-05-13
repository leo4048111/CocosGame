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
		m_allFriendlyBullets.eraseObject(bullet);
		bullet->removeFromParentAndCleanup(1);
	}
	
}

void BulletLayer::update(float delta)
{
	SpriteLayer* spriteLayer = dynamic_cast<SpriteLayer*>(this->getParent()->getChildByName("SpriteLayer"));
	Vector<Target*>* allTargets = spriteLayer->getAllTargets();
	UILayer* uiLayer = dynamic_cast<UILayer*>(this->getParent()->getParent()->getChildByName("UILayer"));
	CrossHair* crossHair = dynamic_cast<CrossHair*>(uiLayer->getChildByName("CrossHair"));
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	Vector<Sprite*> tmpEraseFriendlyBullet;
	Vector<Sprite*> tmpEraseHostileBullet;
	Vector<Target*> tmpEraseTarget;

	//update all friendly bullets and detect collision
	for (auto currentBullet : m_allFriendlyBullets)
	{
		for (auto currentTarget : *allTargets)
		{
			targetType currentTargetType = currentTarget->getTargetType();
			Rect bulletRect = currentBullet->getBoundingBox();
			Rect targetRect = Rect(currentTarget->getBoundingBox().origin, currentTarget->m_sprite->getContentSize() / 5);
			if (bulletRect.intersectsRect(targetRect)) //Collision Detection
			{
				crossHair->showHitNotification(); //show hit anime on crosshair
				if (!currentTarget->receiveDamage(mainCharacter->getCurrentWeapon()->getWeaponDamage()))
				{
					uiLayer->addScore(spriteLayer->getThisTargetScore(currentTarget));
					tmpEraseTarget.pushBack(currentTarget);
				}
				if (currentBullet->getName() != "lazer")
					tmpEraseFriendlyBullet.pushBack(currentBullet);
				break;
			}
		}
	}

	//update all hostile bullets and detect collision
	for (auto currentBullet : m_allHostileBullets)
	{
		Rect bulletRect = currentBullet->getBoundingBox();
		Rect mainCharacterRect = Rect(mainCharacter->getBoundingBox().origin, mainCharacter->m_sprite->getContentSize() / 10.0f);
		if (bulletRect.intersectsRect(mainCharacterRect)) //Collision Detection
		{
			mainCharacter->receiveDamage(10); //has deprecated!
			tmpEraseHostileBullet.pushBack(currentBullet);
		}
	}

	//cleaup bullets
	while (!tmpEraseFriendlyBullet.empty())
	{
		auto tmpBullet = tmpEraseFriendlyBullet.back();
		try
		{
			tmpBullet->stopAllActions();
			tmpEraseFriendlyBullet.popBack();
			m_allFriendlyBullets.eraseObject(tmpBullet);
			tmpBullet->removeFromParentAndCleanup(true);
		}
		catch (const std::exception& exp)
		{
			CCLOG("%s", exp.what());
		}
	}

	while (!tmpEraseHostileBullet.empty())
	{
		auto tmpBullet = tmpEraseHostileBullet.back();
		try
		{
			tmpBullet->stopAllActions();
			tmpEraseHostileBullet.popBack();
			m_allHostileBullets.eraseObject(tmpBullet);
			tmpBullet->removeFromParentAndCleanup(true);
		}
		catch (const std::exception& exp)
		{
			CCLOG("%s", exp.what());
		}
	}

	//cleanup targets
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
	auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet);
	m_allFriendlyBullets.pushBack(bullet);

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

	//DEBUG END

	auto action = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x)), sin(atan2(dst.y, dst.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("bullet");

}

void BulletLayer::addLazer()
{
	//create bullet
	auto lazer = Sprite::create("objects/ammo/ammo_lazer.png");
	lazer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(lazer);
	m_allFriendlyBullets.pushBack(lazer);

	//set bullet rotation
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	CrossHair* crossHair = dynamic_cast<CrossHair*>(this->getParent()->getParent()->getChildByName("UILayer")->getChildByName("CrossHair"));
	Vec2 mousePosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 weaponPosVec = mainCharacter->getPosition();
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	lazer->setRotation(degree);
	lazer->setScale(0.5f);
	lazer->setPosition(mainCharacter->getPosition());

	auto action = FadeIn::create(0.2f);
	auto action2 = FadeOut::create(0.5f);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, action2,callFunc, NULL);
	lazer->runAction(Repeat::create(sequence, 1));
	lazer->setName("lazer");
}

void BulletLayer::addSprayBullet()
{
	const double offsetAngle = 0.418;

	//set bullet rotation
	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	CrossHair* crossHair = dynamic_cast<CrossHair*>(this->getParent()->getParent()->getChildByName("UILayer")->getChildByName("CrossHair"));
	Vec2 mousePosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 weaponPosVec = mainCharacter->getPosition();
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);

	//create bullet
	auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet);
	m_allFriendlyBullets.pushBack(bullet);
	bullet->setRotation(degree);
	bullet->setScale(0.2f);
	bullet->setPosition(mainCharacter->getPosition());

	auto action = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x)), sin(atan2(dst.y, dst.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("sprayBullet");

	//create bullet1
	auto bullet1 = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet1);
	m_allFriendlyBullets.pushBack(bullet1);
	bullet1->setRotation(degree+30);
	bullet1->setScale(0.2f);
	bullet1->setPosition(mainCharacter->getPosition());

	auto action1 = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x)+ offsetAngle), sin(atan2(dst.y, dst.x)+ offsetAngle)));
	CallFuncN* callFunc1 = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence1 = Sequence::create(action1, callFunc1, NULL);
	bullet1->runAction(Repeat::create(sequence1, 1));
	bullet1->setName("sprayBullet");

	//create bullet2
	auto bullet2 = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet2);
	m_allFriendlyBullets.pushBack(bullet2);
	bullet2->setRotation(degree + 15);
	bullet2->setScale(0.2f);
	bullet2->setPosition(mainCharacter->getPosition());

	auto action2 = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x) - offsetAngle), sin(atan2(dst.y, dst.x) - offsetAngle)));
	CallFuncN* callFunc2 = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence2 = Sequence::create(action2, callFunc2, NULL);
	bullet2->runAction(Repeat::create(sequence2, 1));
	bullet2->setName("sprayBullet");

}

void BulletLayer::addSpiritualShockWave(Node* sender)
{
	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
	bullet->setScale(2.0f);
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	MainCharacter* mainCharacter = dynamic_cast<MainCharacter*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName("MainCharacter"));
	Vec2 targetPosVec = sender->getPosition();
	Vec2 mainCharacterPosVec = mainCharacter->getPosition();
	Vec2 dst = mainCharacterPosVec - targetPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);

	bullet->setRotation(degree);
	bullet->setPosition(sender->getPosition());

	auto action = MoveTo::create(TARGET_BULLET_SPEED,1024 * Vec2(cos(atan2(dst.y, dst.x)), sin(atan2(dst.y, dst.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("hostilebBullet");
}