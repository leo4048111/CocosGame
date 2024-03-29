#include "BulletLayer.h"
#include "SpriteLayer.h"
#include "objects/CrossHair.h"
#include "UILayer.h"
#include "objects/MiniMap.h"
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
			m_chargeSpiritAnime.pushBack(cache->getSpriteFrameByName("effect_chargeSpirit_" + Value(c).asString()));
		}
		cache->addSpriteFramesWithFile("objects/effect/effect_flameCircle.plist");
		for (int c = 0; c <= 5; c++)
		{
			m_flameCircleAnime.pushBack(cache->getSpriteFrameByName("effect_flameCircle_" + Value(c).asString()));

		}
		cache->addSpriteFramesWithFile("objects/effect/effect_subterrainAssuat.plist");
		for (int c = 0; c <= 5; c++)
		{
			m_subterrainAssualtAnime.pushBack(cache->getSpriteFrameByName("effect_subterrainAssual_" + Value(c).asString()));
		}
		cache->addSpriteFramesWithFile("objects/effect/effect_bomb.plist");
		for (int c = 0; c <= 11; c++)
		{
			m_bombAnime.pushBack(cache->getSpriteFrameByName("effect_bomb_" + Value(c).asString()));
		}
		cache->addSpriteFramesWithFile("objects/effect/effect_hit1.plist");
		for (int c = 0; c <= 8; c++)
		{
			m_hit1Anime.pushBack(cache->getSpriteFrameByName("effect_hit1_" + Value(c).asString()));
		}
		cache->addSpriteFramesWithFile("objects/effect/effect_fastStrike.plist");
		for (int c = 0; c <= 13; c++)
		{
			m_fastStrikeAnime.pushBack(cache->getSpriteFrameByName("effect_fastStrike_" + Value(c).asString()));
		}
		cache->addSpriteFramesWithFile("objects/effect/effect_flame.plist");
		for (int c = 0; c <= 5; c++)
		{
			m_flameThrower.pushBack(cache->getSpriteFrameByName("effect_flame_" + Value(c).asString()));
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
	auto allTargets = spriteLayer->getAllTargets();
	auto allPlayers = spriteLayer->getAllPlayers();
	auto allObstacles = spriteLayer->getObstacles();
	UILayer* uiLayer = UILayer::getInstance();
	CrossHair* crossHair = CrossHair::getInstance();
	/*Player* player = dynamic_cast<Player*>(this->getParent()->getChildByName("SpriteLayer")->getChildByName(Specs::getInstance()->getPlayerName()));*/
	
	Vector<Sprite*> tmpEraseFriendlyBullet;
	Vector<Sprite*> tmpEraseHostileBullet;
	Vector<Target*> tmpEraseTarget;
	Vector<Entity*> tmpErasePlayer;

	if (allPlayers.front() == NULL)
		return;

	//update all friendly bullets and detect collision
	for (auto currentBullet : m_allFriendlyBullets)
	{
		bool isHit = false;
		for (auto ob : allObstacles) //detect ob collision
		{
			Rect bulletRect = currentBullet->getBoundingBox();
			Rect obRect = Rect(ob->getBoundingBox().origin, ob->m_sprite->getContentSize() / 5);
			if (bulletRect.intersectsRect(obRect)) //Collision Detection
			{
				crossHair->showHitNotification(); //show hit anime on crosshair

				std::string str = currentBullet->getName();
				if (str != "lazer" && str != "flame" && str != "meleeAttack" && str != "toxicBomb")
				{
					tmpEraseFriendlyBullet.pushBack(currentBullet);
					isHit = true;
					break;
				}
			}
		}

		if (isHit)
			continue;

		for (auto currentTarget : allTargets)
		{
			targetType currentTargetType = currentTarget->getTargetType();
			Rect bulletRect = currentBullet->getBoundingBox();
			Rect targetRect = Rect(currentTarget->getBoundingBox().origin, currentTarget->m_sprite->getContentSize() / 5);
			if (bulletRect.intersectsRect(targetRect)) //Collision Detection
			{
				crossHair->showHitNotification(); //show hit anime on crosshair
				if (!currentTarget->receiveDamage(m_bulletDamageMap[currentBullet->getName()]))
				{
					runEffect(currentTarget, m_hit1Anime);
					Specs::getInstance()->addScore(spriteLayer->getThisTargetScore(currentTarget));
					tmpEraseTarget.pushBack(currentTarget);
				}

				std::string str = currentBullet->getName();
				if (str != "lazer" && str != "flame" && str != "meleeAttack"&&str!="toxicBomb")
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
		bool isHit = false;
		for (auto ob : allObstacles)
		{
			Rect bulletRect = currentBullet->getBoundingBox();
			Rect obRect = Rect(ob->getBoundingBox().origin, ob->m_sprite->getContentSize() / 5);
			if (bulletRect.intersectsRect(obRect)) //Collision Detection
			{
				std::string str = currentBullet->getName();
				if (str != "hostileFlameCircle" && str != "hostileSubterrainAssualt" && str != "hostileFastStrike")
				{
					tmpEraseHostileBullet.pushBack(currentBullet);
					isHit = true;
					break;
				}
			}
		}

		if (isHit)
			continue;

		Rect bulletRect = currentBullet->getBoundingBox();
		for (auto currentPlayer : allPlayers)
		{
			Rect playerRect = Rect(currentPlayer->getBoundingBox().origin, currentPlayer->getContentSize() / 5);
			if (bulletRect.intersectsRect(playerRect)) //Collision Detection
			{
				if (!Specs::getInstance()->isInvincibleActivated())
				{
					if (!currentPlayer->receiveDamage(m_bulletDamageMap[currentBullet->getName()]))
					{
						if (currentPlayer->isMe())
							Specs::getInstance()->setWinOrLose(false);
						else
							tmpErasePlayer.pushBack(currentPlayer);
					}
				}

				std::string str = currentBullet->getName();
				if (str != "hostileFlameCircle" && str != "hostileSubterrainAssualt" && str != "hostileFastStrike")
				{
					tmpEraseHostileBullet.pushBack(currentBullet);
					break;
				}
			}
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
			tmpEraseTarget.popBack();
			spriteLayer->removeTarget(tmpTarget);

			if (!Specs::getInstance()->isSinglePlayer())
			{
				neb::CJsonObject ojson;
				ojson.Add("Type", JsonMsgType::TargetDead);
				ojson.Add("Tag", tmpTarget->getTag());
			}

			tmpTarget->targetDead();
			
		}
	}

	//cleanup targets
	while (!tmpErasePlayer.empty())
	{
		auto tmpPlayer = tmpErasePlayer.back();
		if (tmpPlayer != nullptr)
		{
			tmpErasePlayer.popBack();
			spriteLayer->removePlayer(tmpPlayer);
			MiniMap::getInstance()->removeAi(tmpPlayer->getName());
			if (!Specs::getInstance()->isSinglePlayer())
			{
				neb::CJsonObject ojson;
				ojson.Add("Type", JsonMsgType::PlayerDead);
				ojson.Add("Name", tmpPlayer->getName());
			}

			tmpPlayer->runDeadAction();

		}
	}
}

void BulletLayer::pointBulletTo(Node* obj, Vec2 route,double offset)
{
	float radians = M_PI - atan2(route.y, route.x);
	float degree = CC_RADIANS_TO_DEGREES(radians);
	obj->setRotation(degree+offset);
}

void BulletLayer::addBullet(Node* sender,Vec2 startPos,Vec2 terminalPos)
{
	//create bullet
	auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
	this->addChild(bullet);
	m_allFriendlyBullets.pushBack(bullet);
	
	Vec2 route = terminalPos - startPos;

	//set bullet rotation
	pointBulletTo(bullet, route,0);

	bullet->setScale(0.4f);
	bullet->setPosition(startPos);

	auto action = MoveTo::create(1.0f, startPos + 1024 * Vec2(cos(atan2(route.y, route.x)), sin(atan2(route.y, route.x))));
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, callFunc, NULL);
	bullet->runAction(Repeat::create(sequence, 1));
	bullet->setName("bullet");

}

void BulletLayer::addLazer(Node* sender,Vec2 startPos, Vec2 terminalPos)
{
	//create bullet
	auto lazer = Sprite::create("objects/ammo/ammo_lazer.png");
	lazer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(lazer);
	m_allFriendlyBullets.pushBack(lazer);

	Vec2 route = terminalPos - startPos;

	//set bullet rotation
	pointBulletTo(lazer, route,0);

	lazer->setScale(0.5f);
	lazer->setPosition(startPos);

	auto action = FadeIn::create(0.2f);
	auto action2 = FadeOut::create(0.5f);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(action, action2, callFunc,NULL);
	lazer->runAction(Repeat::create(sequence, 1));
	lazer->setName("lazer");
}

void BulletLayer::addSprayBullet(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double offsetAngle = 0.418;

	Vec2 route = terminalPos - startPos;

	//create bullet X 3
	for (int offset = -15; offset <= 15; offset += 15)
	{
		auto bullet = Sprite::create("objects/ammo/ammo_normal.png");
		this->addChild(bullet);
		m_allFriendlyBullets.pushBack(bullet);
		
		//set bullet rotation
		pointBulletTo(bullet, route, offset);

		bullet->setScale(0.4f);
		bullet->setPosition(startPos);

		auto action = MoveTo::create(1.0f, startPos + 1024 * Vec2(cos(atan2(route.y, route.x)+offset/180.0*M_PI), sin(atan2(route.y, route.x)+offset / 180.0 * M_PI)));
		CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
		auto sequence = Sequence::create(action, callFunc, NULL);
		bullet->runAction(sequence);
		bullet->setName("sprayBullet");
	}

}

void BulletLayer::addToxicBomb(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	//create bomb sprite
	auto bomb = Sprite::create("objects/ammo/ammo_toxicBomb.png");
	bomb->setScale(0.4f);
	this->addChild(bomb);
	m_allFriendlyBullets.pushBack(bomb);

	Vec2 route = terminalPos - startPos;

	//set bullet rotation
	pointBulletTo(bomb, route, 0);

	bomb->setScale(0.5f);
	bomb->setPosition(startPos);

	auto action1 = RepeatForever::create(RotateTo::create(0.5f, 1280));
	auto action2 = JumpTo::create(1.0f, terminalPos, 3.0f, 2);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto spawn = Spawn::create(action1, action2, NULL);

	auto animation = Animation::createWithSpriteFrames(m_bombAnime, 0.5f / m_bombAnime.size());
	auto animate = Animate::create(animation);
	auto action4 = Repeat::create(animate, 1);

	auto sequence = Sequence::create(spawn, action4, callFunc, NULL);

	bomb->runAction(sequence);
	bomb->setName("toxicBomb");
}

void BulletLayer::addFlameThrower(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double c_flameSurvivalDuration= 0.5f;
	auto flame = Sprite::create("objects/ammo/ammo_flame.png");
	flame->setScale(1.0f);
	flame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(flame);
	m_allFriendlyBullets.pushBack(flame);

	Vec2 route = terminalPos - startPos;

	//set bullet rotation
	pointBulletTo(flame, route, 0);

	flame->setPosition(startPos);

	//flame anime and auto cleanup
	auto animation = Animation::createWithSpriteFrames(m_flameThrower, 0.5f / m_flameThrower.size());
	auto animate = Animate::create(animation);
	auto action4 = Repeat::create(animate, 1);

	auto fadein = FadeIn::create(c_flameSurvivalDuration / 2);
	auto fadeout = FadeOut::create(c_flameSurvivalDuration / 2);
	auto seq = Sequence::create(fadein, fadeout, NULL);
	auto spawn = Spawn::create(seq, action4,NULL);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(spawn, callFunc,NULL);

	flame->runAction(sequence);
	flame->setName("flame");

}

void BulletLayer::addMeleeAttack(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double c_effectDuration = 0.2f;
	auto effect = Sprite::create("objects/ammo/ammo_slash.png");
	effect->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	this->addChild(effect);
	m_allFriendlyBullets.pushBack(effect);

	Vec2 route = terminalPos - startPos;

	//set bullet rotation
	pointBulletTo(effect, route, 0);

	effect->setScale(0.3f);
	effect->setPosition(startPos);

	//flame anime and auto cleanup
	auto fadein = FadeIn::create(c_effectDuration / 2);
	auto fadeout = FadeOut::create(c_effectDuration / 2);
	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet));
	auto sequence = Sequence::create(fadein, fadeout, callFunc, NULL);

	effect->runAction(sequence);

	effect->setName("meleeAttack");
}

void BulletLayer::initHostileBulletDamageMap()
{
	m_bulletDamageMap.insert(std::make_pair("hostileSpiritualPower", 30));
	m_bulletDamageMap.insert(std::make_pair("hostileFlameCircle", 0.5f));
	m_bulletDamageMap.insert(std::make_pair("hostileSubterrainAssualt", 0.25f));
	m_bulletDamageMap.insert(std::make_pair("hostileFastStrike", 0.25f));
	m_bulletDamageMap.insert(std::make_pair("bullet", 20.0f));
	m_bulletDamageMap.insert(std::make_pair("lazer", 1.5f));
	m_bulletDamageMap.insert(std::make_pair("sprayBullet", 15.0f));
	m_bulletDamageMap.insert(std::make_pair("toxicBomb", 0.7f));
	m_bulletDamageMap.insert(std::make_pair("flame", 0.5f));
	m_bulletDamageMap.insert(std::make_pair("meleeAttack", 2.0f));

}

void BulletLayer::addSpiritualPower(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	runEffect(sender, m_chargeSpiritAnime);

	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_spiritualPower.png");
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Vec2 thisStartPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(startPos));
	Vec2 thisTerminalPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(terminalPos));
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
	runEffect(sender,m_flameCircleAnime);

	const double c_totalExpessionPeriodLength = 1.5f;
	//create bullet sprite
	auto bullet = Sprite::create("objects/ammo/ammo_flameCircle.png");
	this->addChild(bullet);
	m_allHostileBullets.pushBack(bullet);

	Vec2 thisStartPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(startPos));
	Vec2 thisTerminalPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(terminalPos));
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	pointBulletTo(bullet, route, 0);
	bullet->setScale(0.3f);
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

void BulletLayer::addSubterrainAssualt(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double c_totalExpessionPeriodLength = 1.5f;
	//create bullet sprite
	auto bullet = Sprite::create("objects/UI/ui_attackWarning.png");
	this->addChild(bullet);

	m_allHostileBullets.pushBack(bullet);

	Vec2 thisStartPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(startPos));
	Vec2 thisTerminalPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(terminalPos));
	Vec2 route = thisTerminalPos - thisStartPos;

	//set bullet rotation
	bullet->setScale(0.4f);
	bullet->setPosition(thisTerminalPos);

	auto fadein = FadeIn::create(0.5f);
	auto fadeout = FadeOut::create(0.5f);
	auto sequence = Sequence::create(fadeout,fadein, NULL);
	auto glow = Repeat::create(sequence, 3);
	
	auto animation = Animation::createWithSpriteFrames(m_subterrainAssualtAnime, 0.5f / m_subterrainAssualtAnime.size());
	auto animate = Animate::create(animation);
	auto action = Repeat::create(animate,2);

	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet)); //cleanup

	auto sequence1 = Sequence::create(glow, action, callFunc,NULL);
	bullet->setName("hostileSubterrainAssualt");
	bullet->runAction(sequence1);
}

void BulletLayer::addFastStrike(Node* sender, Vec2 startPos, Vec2 terminalPos)
{
	const double c_totalExpessionPeriodLength = 1.5f;
	//create bullet sprite
	auto strike = Sprite::create("objects/ammo/ammo_slash.png");
	this->addChild(strike);
	m_allHostileBullets.pushBack(strike);

	Vec2 thisStartPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(startPos));
	Vec2 thisTerminalPos = this->convertToNodeSpace(SpriteLayer::getInstance()->convertToWorldSpace(terminalPos));
	Vec2 route = thisTerminalPos - thisStartPos;

	strike->setPosition(thisTerminalPos);

	auto animation = Animation::createWithSpriteFrames(m_fastStrikeAnime, 0.5f / m_fastStrikeAnime.size());
	auto animate = Animate::create(animation);
	auto action = Repeat::create(animate, 2);

	CallFuncN* callFunc = CallFuncN::create(this, callfuncN_selector(BulletLayer::cleanBullet)); //cleanup

	auto sequence1 = Sequence::create(action, callFunc, NULL);
	strike->runAction(sequence1);
	strike->setName("hostileFastStrike");
}