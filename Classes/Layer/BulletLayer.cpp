#include "BulletLayer.h"
#include "SpriteLayer.h"
#include "objects/CrossHair.h"
#include "UILayer.h"
#include "Controls/Specs.h"

USING_NS_CC;

BulletLayer* BulletLayer::_instance = NULL;

BulletLayer* BulletLayer::createBulletLayer()
{
	if(_instance == NULL)
	_instance = BulletLayer::create();
	return _instance;
}

bool BulletLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	initHostileBulletDamageMap();

	this->setName("BulletLayer");
	return true;
}

void BulletLayer::cleanBullet(Node* sender)
{
	Sprite* bullet = dynamic_cast<Sprite*>(sender);
	if (bullet != nullptr)
	{
		m_allFriendlyBullets.eraseObject(bullet);
		m_allHostileBullets.eraseObject(bullet);
		bullet->removeFromParentAndCleanup(true);
	}
	
}

void BulletLayer::update(float delta)
{
	SpriteLayer* spriteLayer = SpriteLayer::getInstance();
	Vector<Target*>* allTargets = spriteLayer->getAllTargets();
	UILayer* uiLayer = UILayer::getInstance();
	CrossHair* crossHair = CrossHair::getInstance();
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	Vector<Sprite*> tmpEraseFriendlyBullet;
	Vector<Sprite*> tmpEraseHostileBullet;
	Vector<Target*> tmpEraseTarget;

	if (player == NULL)
		return;

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
				currentTarget->setColor(Color3B(245, 2, 1));
				crossHair->showHitNotification(); //show hit anime on crosshair
			if (!currentTarget->receiveDamage(player->getCurrentWeapon()->getWeaponDamage()))
				{
					Specs::getInstance()->addScore(spriteLayer->getThisTargetScore(currentTarget));
					tmpEraseTarget.pushBack(currentTarget);
				}
				if (currentBullet->getName() != "lazer" && currentBullet->getName() != "flame")
				{
					tmpEraseFriendlyBullet.pushBack(currentBullet);
					break;
				}
			}
		}
	}

	//update all hostile bullets and detect collision
	for (auto currentBullet : m_allHostileBullets)
	{
		Rect bulletRect = currentBullet->getBoundingBox();
		Rect mainCharacterRect = Rect(player->getBoundingBox().origin, player->m_sprite->getContentSize() / 10.0f);
		if (bulletRect.intersectsRect(mainCharacterRect)) //Collision Detection
		{
			if (!Specs::getInstance()->isInvincibleActivated())
			{
				if (!player->receiveDamage(m_hostileBulletDamageMap[currentBullet->getName()]))
				{
					/*auto callFunc = CallFunc::create(CC_CALLBACK_0(Entity::runDeadAction, player));
					auto callFunc2 = CallFuncN::create(CC_CALLBACK_0(Specs::setWinOrLose, Specs::getInstance(), false));
					CCFiniteTimeAction* sequence = Sequence::create(callFunc,DelayTime::create(1.0f),callFunc2,NULL);
					player->runAction(sequence);*/
					Specs::getInstance()->setWinOrLose(false);
				}
			}
			if(currentBullet->getName()!="hostileFlameCircle")
			tmpEraseHostileBullet.pushBack(currentBullet);
		}
	}

	//cleaup bullets
	while (!tmpEraseFriendlyBullet.empty())
	{
		auto tmpBullet = tmpEraseFriendlyBullet.back();
		try
		{
			if (tmpBullet != nullptr)
			{
				tmpBullet->stopAllActions();
				tmpEraseFriendlyBullet.popBack();
				m_allFriendlyBullets.eraseObject(tmpBullet);
				tmpBullet->removeFromParentAndCleanup(true);
			}
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
			if (tmpBullet != nullptr)
			{
				tmpBullet->stopAllActions();
				tmpEraseHostileBullet.popBack();
				m_allHostileBullets.eraseObject(tmpBullet);
				tmpBullet->removeFromParentAndCleanup(true);
			}
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
			//speak some lines
			tmpTarget->speak(Specs::getInstance()->speakRandom());

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
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	CrossHair* crossHair = CrossHair::getInstance();
	Vec2 mousePosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 weaponPosVec = mainCharacter->getPosition();
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	bullet->setRotation(degree);
	bullet->setScale(0.2f);
	bullet->setPosition(mainCharacter->getPosition());

	////DEBUG
	//std::string str1 = "ch:" + Value(mainCharacter->getPosition().x).asString() + "," + Value(mainCharacter->getPosition().y).asString() + "\n";
	//OutputDebugString(str1.c_str());
	//std::string str2 = "bullet:" + Value(bullet->getPosition().x).asString() + "," + Value(bullet->getPosition().y).asString() + "\n";
	//OutputDebugString(str2.c_str());
	//std::string str4 = "mouse:" + Value(mousePosVec.x).asString() + "," + Value(mousePosVec.y).asString() + "\n";
	//OutputDebugString(str4.c_str());
	//std::string str5 = "dst:" + Value(dst.x).asString() + "," + Value(dst.y).asString() + "\n";
	//OutputDebugString(str5.c_str());
	//std::string str3 = mainCharacter->getName() + "\n";
	//OutputDebugString(str3.c_str());
	//SpriteLayer* spriteLayer = dynamic_cast<SpriteLayer*>(this->getParent()->getChildByName("SpriteLayer"));
	//Vector<Target*>* allTargets = spriteLayer->getAllTargets();

	////DEBUG END

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
	Player* mainCharacter = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	CrossHair* crossHair = CrossHair::getInstance();
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
	auto sequence = Sequence::create(action, action2, callFunc,NULL);
	lazer->runAction(Repeat::create(sequence, 1));
	lazer->setName("lazer");
}

void BulletLayer::addSprayBullet()
{
	const double offsetAngle = 0.418;

	//set bullet rotation
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	CrossHair* crossHair = CrossHair::getInstance();
	Vec2 mousePosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 weaponPosVec = player->getPosition();
	Vec2 dst = mousePosVec - weaponPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);

	//create bullet X 3
	for (int offset = -15; offset <= 15; offset += 15)
	{
		auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
		this->addChild(bullet);
		m_allFriendlyBullets.pushBack(bullet);
		bullet->setRotation(degree+ offset);
		bullet->setScale(0.2f);
		bullet->setPosition(weaponPosVec);

		auto action = MoveTo::create(1.0f, 1024 * Vec2(cos(atan2(dst.y, dst.x)+offset/180.0*M_PI), sin(atan2(dst.y, dst.x)+offset / 180.0 * M_PI)));
		CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
		auto sequence = Sequence::create(action, callFunc, NULL);
		bullet->runAction(Repeat::create(sequence, 1));
		bullet->setName("sprayBullet");
	}

}

void BulletLayer::addToxicBomb()
{
	//create bomb sprite
	auto bomb = Sprite::create("objects/ammo/ammo_toxicBomb.png");
	bomb->setScale(0.4f);
	this->addChild(bomb);

	//set bomb position and dst
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	CrossHair* crossHair = CrossHair::getInstance();
	Vec2 dstPosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 originPosVec = player->getPosition();
	Vec2 dst = dstPosVec - originPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	bomb->setRotation(degree);
	bomb->setPosition(player->getPosition());

	auto action1 = RepeatForever::create(RotateTo::create(0.5f, 90));
	auto action2 = JumpTo::create(1.0f, dstPosVec, 3.0f, 2);
	auto action3 = FadeOut::create(0.5f);
	auto spawn = Spawn::create(action1, action2, NULL);
	auto sequence = Sequence::create(spawn, action3, NULL);
	bomb->runAction(sequence);
	bomb->setName("toxicBomb");
}

void BulletLayer::addFlameThrower()
{
	const double c_flameSurvivalDuration= 0.2f;
	auto flame = Sprite::create("objects/ammo/ammo_flame.png");
	flame->setScale(0.3f);
	flame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(flame);
	m_allFriendlyBullets.pushBack(flame);

	//set bomb position and dst
	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	CrossHair* crossHair = CrossHair::getInstance();
	Vec2 dstPosVec = this->convertToNodeSpace(crossHair->getCursorPos());
	Vec2 originPosVec = player->getPosition();
	Vec2 dst = dstPosVec - originPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);

	flame->setRotation(degree);
	flame->setPosition(player->getPosition());

	//flame anime and auto cleanup
	auto fadein = FadeIn::create(c_flameSurvivalDuration / 2);
	auto fadeout = FadeOut::create(c_flameSurvivalDuration / 2);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(fadein, fadeout, callFunc,NULL);

	flame->runAction(sequence);
	flame->setName("flame");

}

void BulletLayer::initHostileBulletDamageMap()
{
	m_hostileBulletDamageMap.insert(std::make_pair("hostileSpiritualPower", 30));
	m_hostileBulletDamageMap.insert(std::make_pair("hostileFlameCircle", 2.0f));
}

void BulletLayer::addSpiritualPower(Node* sender)
{
	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_spiritualPower.png");
	bullet->setScale(0.1f);
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));
	Vec2 targetPosVec = sender->getPosition();
	Vec2 playerPosVec = player->getPosition();
	Vec2 dst = playerPosVec - targetPosVec;
	float radians = M_PI - atan2(dst.y, dst.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);

	bullet->setRotation(degree);
	bullet->setPosition(sender->getPosition());

	auto action = MoveTo::create(TARGET_BULLET_SPEED,1024 * Vec2(cos(atan2(dst.y, dst.x)), sin(atan2(dst.y, dst.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("hostileSpiritualPower");
}

void BulletLayer::addFlameCircle(Node* sender)
{
	const double c_totalExpessionPeriodLength = 1.5f;
	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_flameCircle.png");
	bullet->setScale(0.2f);
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Vec2 targetPosVec = sender->getPosition();

	bullet->setPosition(targetPosVec);

	auto scaleby = ScaleBy::create(c_totalExpessionPeriodLength, 10.0f); //zoom out
	auto rotateby = RotateBy::create(c_totalExpessionPeriodLength, 360 * 10); //rotate by
	auto fadein = FadeIn::create(c_totalExpessionPeriodLength/2); //the initial half period
	auto fadeout = FadeOut::create(c_totalExpessionPeriodLength/2); //Fade out
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet)); //cleanup

	auto sequence = Sequence::create(fadein, fadeout,callFunc,NULL);
	auto spawn = Spawn::create(scaleby, rotateby,sequence, NULL);

	bullet->setName("hostileFlameCircle");
	bullet->runAction(spawn);
}

