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
	if (!loadGraphs())
	{
		return false;
	}

	initHostileBulletDamageMap();

	this->setName("BulletLayer");
	return true;
}

bool BulletLayer::loadGraphs()
{
	try
	{
		auto cache = SpriteFrameCache::getInstance();

		cache->addSpriteFramesWithFile("objects/effect/effect_explotion.plist");
		/*for (int c = 1; c <= 64; c++)
		{
			m_explodeAnime.pushBack(cache->getSpriteFrameByName("effect_explotion_" + Value(c).asString()));
		}*/
		cache->addSpriteFramesWithFile("objects/effect/effect_chargeSpirit.plist");
		for (int c = 0; c <= 28; c++)
		{
			m_chargeSpiritAnime.pushBack(cache->getSpriteFrameByName("effect_chargeSpirit" + Value(c).asString()));
		}

		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

void BulletLayer::runEffect(Node* sender,Vector<SpriteFrame*> effectFrame)
{
	auto effect = Sprite::createWithSpriteFrame(effectFrame.front());
	effect->setScale(0.5f);
	auto animation = Animation::createWithSpriteFrames(effectFrame, 0.5f / effectFrame.size());
	auto animate = Animate::create(animation);
	auto effectAction = Repeat::create(animate, 1);
	sender->addChild(effect, 100);
	auto effectCallFunc = CallFunc::create(CC_CALLBACK_0(Sprite::removeFromParent, effect));
	auto effectSequence = Sequence::create(effectAction, effectCallFunc, NULL);
	effect->runAction(effectSequence);
}

void BulletLayer::cleanBullet(Node* sender)
{
	Sprite* bullet = dynamic_cast<Sprite*>(sender);
	if (bullet != NULL)
	{
		m_allFriendlyBullets.eraseObject(bullet);
		m_allHostileBullets.eraseObject(bullet);
		bullet->removeFromParent();
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
				crossHair->showHitNotification(); //show hit anime on crosshair
				if (!currentTarget->receiveDamage(player->getCurrentWeapon()->getWeaponDamage()))
				{
					Specs::getInstance()->addScore(spriteLayer->getThisTargetScore(currentTarget));
					tmpEraseTarget.pushBack(currentTarget);
				}

				std::string str = currentBullet->getName();
				if (str != "lazer" && str != "flame" && str != "meleeAttack")
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
			if (tmpBullet != NULL)
			{
				tmpBullet->stopAllActions();
				tmpEraseFriendlyBullet.popBack();
				m_allFriendlyBullets.eraseObject(tmpBullet);
				tmpBullet->removeFromParent();
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
			if (tmpBullet != NULL)
			{
				tmpBullet->stopAllActions();
				tmpEraseHostileBullet.popBack();
				m_allHostileBullets.eraseObject(tmpBullet);
				tmpBullet->removeFromParent();
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

void BulletLayer::pointBulletTo(Node* obj, Vec2 route,double offset)
{
	float radians = M_PI - atan2(route.y, route.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	obj->setRotation(degree+offset);
}

void BulletLayer::addBullet(Vec2 startPos,Vec2 terminalPos)
{
	//create bullet
	auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet);
	m_allFriendlyBullets.pushBack(bullet);
	
	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(bullet, route,0);

	bullet->setScale(0.2f);
	bullet->setPosition(thisStartPos);

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


	auto action = MoveTo::create(1.0f, thisStartPos+ 1024 * Vec2(cos(atan2(route.y, route.x)), sin(atan2(route.y, route.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("bullet");

}

void BulletLayer::addLazer(Vec2 startPos, Vec2 terminalPos)
{
	//create bullet
	auto lazer = Sprite::create("objects/ammo/ammo_lazer.png");
	lazer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(lazer);
	m_allFriendlyBullets.pushBack(lazer);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(lazer, route,0);

	lazer->setScale(0.5f);
	lazer->setPosition(thisStartPos);

	auto action = FadeIn::create(0.2f);
	auto action2 = FadeOut::create(0.5f);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, action2, callFunc,NULL);
	lazer->runAction(Repeat::create(sequence, 1));
	lazer->setName("lazer");
}

void BulletLayer::addSprayBullet(Vec2 startPos, Vec2 terminalPos)
{
	const double offsetAngle = 0.418;

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//create bullet X 3
	for (int offset = -15; offset <= 15; offset += 15)
	{
		auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
		this->addChild(bullet);
		m_allFriendlyBullets.pushBack(bullet);
		
		//set bullet rotation
		pointBulletTo(bullet, route, offset);

		bullet->setScale(0.2f);
		bullet->setPosition(thisStartPos);

		auto action = MoveTo::create(1.0f, thisStartPos+ 1024 * Vec2(cos(atan2(route.y, route.x)+offset/180.0*M_PI), sin(atan2(route.y, route.x)+offset / 180.0 * M_PI)));
		CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
		auto sequence = Sequence::create(action, callFunc, NULL);
		bullet->runAction(sequence);
		bullet->setName("sprayBullet");
	}

}

void BulletLayer::addToxicBomb(Vec2 startPos, Vec2 terminalPos)
{
	//create bomb sprite
	auto bomb = Sprite::create("objects/ammo/ammo_toxicBomb.png");
	bomb->setScale(0.4f);
	this->addChild(bomb);
	m_allFriendlyBullets.pushBack(bomb);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(bomb, route, 0);

	bomb->setScale(0.5f);
	bomb->setPosition(thisStartPos);

	auto action1 = RepeatForever::create(RotateTo::create(0.5f, 1280));
	auto action2 = JumpTo::create(1.0f, thisTerminalPos, 3.0f, 2);
	auto action3 = FadeOut::create(0.5f);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto spawn = Spawn::create(action1, action2, NULL);
	auto animation = Animation::createWithSpriteFrames(m_explodeAnime, 1.0f);
	auto animate = Animate::create(animation);
	auto action4 = Repeat::create(animate, 10);
	auto sequence = Sequence::create(spawn, action3, action4, callFunc, NULL);
	bomb->runAction(sequence);
	bomb->setName("toxicBomb");
}

void BulletLayer::addFlameThrower(Vec2 startPos, Vec2 terminalPos)
{
	const double c_flameSurvivalDuration= 0.2f;
	auto flame = Sprite::create("objects/ammo/ammo_flame.png");
	flame->setScale(0.3f);
	flame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(flame);
	m_allFriendlyBullets.pushBack(flame);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(flame, route, 0);

	flame->setPosition(thisStartPos);

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

void BulletLayer::addSpiritualPower(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	runEffect(sender, m_chargeSpiritAnime);

	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_spiritualPower.png");
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(bullet, route, 0);
	bullet->setScale(0.1f);
	bullet->setPosition(thisStartPos);

	auto action = MoveTo::create(TARGET_BULLET_SPEED, thisStartPos+1024 * Vec2(cos(atan2(route.y, route.x)), sin(atan2(route.y, route.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("hostileSpiritualPower");
}

void BulletLayer::addFlameCircle(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double c_totalExpessionPeriodLength = 1.5f;
	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_flameCircle.png");
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(bullet, route, 0);
	bullet->setScale(0.2f);
	bullet->setPosition(thisStartPos);

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

void BulletLayer::addMeleeAttack(Vec2 startPos, Vec2 terminalPos)
{
	const double c_effectDuration = 0.2f;
	auto effect = Sprite::create("objects/ammo/ammo_slash.png");
	effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(effect);
	m_allFriendlyBullets.pushBack(effect);

	Vec2 thisStartPos = this->convertToNodeSpace(startPos);
	Vec2 thisTerminalPos = this->convertToNodeSpace(terminalPos);
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(effect, route, 0);

	effect->setScale(0.3f);
	effect->setPosition(thisStartPos);

	//flame anime and auto cleanup
	auto fadein = FadeIn::create(c_effectDuration / 2);
	auto fadeout = FadeOut::create(c_effectDuration / 2);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(fadein, fadeout, callFunc, NULL);

	effect->runAction(sequence);

	effect->setName("meleeAttack");
}