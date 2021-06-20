#include "Target.h"
#include "Controls/Specs.h"
#include "Layer/UILayer.h"
#include "Layer/SpriteLayer.h"
#include "Objects/MiniMap.h"
#include "Network/SocketServer.h"
#include "CJsonObject/CJsonObject.hpp"
#include <vector>

USING_NS_CC;
static int tagId = 1;

Target* Target::createTarget()
{
	return Target::create();
}

bool Target::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/target/target.plist");
		return true;
	}
	catch (const std::exception& exp)
	{
		CCLOG("%s", exp.what());
		return false;
	}
}

bool Target::init()
{
	if (!Entity::init())
	{
		return false;
	}
	if (!Target::loadGraphs())
	{
		return false;
	}

	m_lastUpdateTime = time(NULL);
	m_currentDir = moveleft;
	this->setTargetType((targetType)(random() % 4));
	Target::showHealthBar();
	this->setTag(tagId++);

	return true;
}

void Target::update(float delta)
{
	if (this->getCurrentStamina() == ENTITY_MAX_STAMINA)
	{
		//init route
		auto allPlayers = SpriteLayer::getInstance()->getAllPlayers();
		Entity* player = allPlayers[random() % (allPlayers.size())];
		Vec2 startPos = this->getPosition();
		Vec2 terminalPos = player->getPosition();
		this->attack(startPos, terminalPos);
	}

	//Target always moves towards main character
	double offset = 145*this->getCurrentSpeed();
	auto mainCharacter=this->getParent()->getChildByName(Specs::getInstance()->getPlayerName());
	Vec2 mainCharacterPos = mainCharacter->getParent()->convertToWorldSpaceAR(mainCharacter->getPosition());
	Vec2 targetPos = this->getParent()->convertToWorldSpaceAR(this->getPosition());
	Vec2 dst = mainCharacterPos - targetPos;
	double offsetX = offset * cos(atan2(dst.y, dst.x));
	double offsetY = offset * sin(atan2(dst.y, dst.x));

	if (offsetX < 0)
	{
		this->m_sprite->stopActionByTag(moveright);
		this->m_sprite->stopActionByTag(attackright);
		if (!m_isAttacking)
		{
			this->m_sprite->stopActionByTag(attackleft);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_leftWalkAnime, 1.0f)));
			action->setTag(moveleft);
			this->m_sprite->runAction(action);
		}
		else
		{
			this->m_sprite->stopActionByTag(moveleft);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_leftAttackAnime, 1.0f)));
			action->setTag(attackleft);
			this->m_sprite->runAction(action);
		}
		m_currentDir = moveleft;
		
	}
	else if (offsetX > 0)
	{
		this->m_sprite->stopActionByTag(moveleft);
		this->m_sprite->stopActionByTag(attackleft);
		if (!m_isAttacking)
		{
			this->m_sprite->stopActionByTag(attackright);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_rightWalkAnime, 1.0f)));
			action->setTag(moveright);
			this->m_sprite->runAction(action);
		}
		else
		{
			this->m_sprite->stopActionByTag(moveright);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_rightAttackAnime, 1.0f)));
			action->setTag(attackright);
			this->m_sprite->runAction(action);
		}
		m_currentDir = moveright;
	}

	//update position every 1 sec
	time_t currentTime = time(NULL);
	if (currentTime - m_lastUpdateTime < 1)
	{
		this->addStamina(0.5f);
		return;
	}

	m_lastUpdateTime = currentTime;

	double posX = this->getPosition().x + offsetX;
	double posY = this->getPosition().y + offsetY;
	this->runAction(MoveTo::create(0.5f, Vec2(posX, posY)));
	MiniMap::getInstance()->updateTarget(this->getTag(), Vec2(posX, posY));

	if (Specs::getInstance()->isSinglePlayer())
		return;

	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::TargetData);
	ojson.Add("PosX", posX);
	ojson.Add("PosY", posY);
	ojson.Add("Dir", m_currentDir);
	ojson.Add("Tag", this->getTag());
	SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());

}

void Target::targetDead()
{
	this->speak(Specs::getInstance()->speakRandom());
	this->dropRandomCollectable();
	this->runDeadAction();

	MiniMap::getInstance()->removeTarget(this->getTag());
}

void Target::setTargetType(targetType type)
{
	m_type = type;
	String targetName,targetLeftMoveFrameName, targetRightMoveFrameName,targetLeftAttackFrameName, targetRightAttackFrameName;
	
	switch (m_type)
	{
	case targetType::ghost:
		targetName = "target_ghost";
		targetLeftMoveFrameName = "target_ghost_leftMove";
		targetRightMoveFrameName = "target_ghost_rightMove";
		targetLeftAttackFrameName = "target_ghost_leftAttack";
		targetRightAttackFrameName = "target_ghost_rightAttack";
		break;
	case targetType::jellyGhost:
		targetName = "target_jellyGhost";
		targetLeftMoveFrameName = "target_jellyGhost_leftMove";
		targetRightMoveFrameName = "target_jellyGhost_rightMove";
		targetLeftAttackFrameName = "target_jellyGhost_leftAttack";
		targetRightAttackFrameName = "target_jellyGhost_rightAttack";
		break;
	case targetType::sadGhost:
		targetName = "target_sadGhost";
		targetLeftMoveFrameName = "target_sadGhost_leftMove";
		targetRightMoveFrameName = "target_sadGhost_rightMove";
		targetLeftAttackFrameName = "target_sadGhost_leftAttack";
		targetRightAttackFrameName = "target_sadGhost_rightAttack";
		break;
	case targetType::spirit:
		targetName = "target_spirit";
		targetLeftMoveFrameName = "target_spirit_leftMove";
		targetRightMoveFrameName = "target_spirit_rightMove";
		targetLeftAttackFrameName = "target_spirit_leftAttack";
		targetRightAttackFrameName = "target_spirit_rightAttack";
		break;
	default:
		return;
	}

	//reset target sprite display
	auto target = Sprite::createWithSpriteFrameName(targetName.getCString());
	if (m_sprite != nullptr)
		m_sprite->removeFromParentAndCleanup(1);
	this->setName(targetName.getCString());
	target->setScale(0.1f);
	Target::bindSprite(target);

	//reset target sprite anime frame cache
	auto cache = SpriteFrameCache::getInstance();

	m_leftWalkAnime.clear();
	m_rightWalkAnime.clear();
	m_leftAttackAnime.clear();
	m_rightAttackAnime.clear();

	m_leftWalkAnime.pushBack(cache->getSpriteFrameByName(targetLeftMoveFrameName.getCString()));
	m_rightWalkAnime.pushBack(cache->getSpriteFrameByName(targetRightMoveFrameName.getCString()));
	m_leftAttackAnime.pushBack(cache->getSpriteFrameByName(targetLeftAttackFrameName.getCString()));
	m_rightAttackAnime.pushBack(cache->getSpriteFrameByName(targetRightAttackFrameName.getCString()));

}

void Target::moveWithPos(double posX, double posY, int dir)
{
	this->runAction(MoveTo::create(0.5f, Vec2(posX, posY)));

	if (dir == moveleft)
	{
		this->m_sprite->stopActionByTag(moveright);
		this->m_sprite->stopActionByTag(attackright);
		if (!m_isAttacking)
		{
			this->m_sprite->stopActionByTag(attackleft);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_leftWalkAnime, 1.0f)));
			action->setTag(moveleft);
			this->m_sprite->runAction(action);
		}
		else
		{
			this->m_sprite->stopActionByTag(moveleft);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_leftAttackAnime, 1.0f)));
			action->setTag(attackleft);
			this->m_sprite->runAction(action);
		}
		m_currentDir = moveleft;
	}
	else
	{
		this->m_sprite->stopActionByTag(moveleft);
		this->m_sprite->stopActionByTag(attackleft);
		if (!m_isAttacking)
		{
			this->m_sprite->stopActionByTag(attackright);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_rightWalkAnime, 1.0f)));
			action->setTag(moveright);
			this->m_sprite->runAction(action);
		}
		else
		{
			this->m_sprite->stopActionByTag(moveright);
			auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_rightAttackAnime, 1.0f)));
			action->setTag(attackright);
			this->m_sprite->runAction(action);
		}
		m_currentDir = moveright;
	}
}

targetType Target::getTargetType()
{
	return m_type;
}

void Target::dropSpecificCollectable(collectableType type)
{
	CollectableLayer* collectableLayer = dynamic_cast<CollectableLayer*>(this->getParent()->getChildByName("CollectableLayer"));
	auto collectable = Collectable::createCollectable();
	collectable->setCollectableType(type);
	collectableLayer->addCollectable(collectable,this->getPosition().x,this->getPosition().y);
}

void Target::dropRandomCollectable()
{
	srand((unsigned long long)time(NULL));
	collectableType type = (collectableType)(rand() % collectableType::cflameThrower);
	dropSpecificCollectable(type);
}

void Target::attack(Vec2 startPos,Vec2 terminalPos)
{
	//clear stambar
	this->addStamina(-this->getCurrentStamina());
	m_isAttacking = true;

	//run anime
	auto testScaleBy1 = ScaleBy::create(0.5f, 2.0f);
	auto testScaleBy2 = ScaleBy::create(0.5f, 0.5f);
	CallFuncN* callfunc = CallFuncN::create(this, callfuncN_selector(Target::attackEnd));
	auto sequence = Sequence::create(testScaleBy1, testScaleBy2, callfunc, NULL);
	this->runAction(sequence);

	//run function
	switch (this->getTargetType())
	{
	case targetType::sadGhost:
		fireSpiritualPower(startPos,terminalPos);
		break;
	case targetType::jellyGhost:
		fireFlameCircle(startPos, terminalPos);
		break;
	case targetType::ghost:
		fireSubterrainAssualt(startPos, terminalPos);
		break;
	case targetType::spirit:
	{
		auto moveto = MoveTo::create(0.1f, terminalPos);
		auto fadeout = FadeOut::create(0.05f);
		auto fadein = FadeIn::create(0.05f);
		auto sequence = Sequence::create(fadeout, fadein, NULL);
		auto spawn = Spawn::create(moveto, sequence, NULL);
		this->runAction(spawn);
		doFastStrike(startPos, terminalPos);
	}
		break;
	default:
		fireSpiritualPower(startPos, terminalPos);
		break;
	}


	if (Specs::getInstance()->isSinglePlayer())
		return;

	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::TargetAttack);
	ojson.Add("Tag", this->getTag());
	ojson.Add("StartX", startPos.x);
	ojson.Add("StartY", startPos.y);
	ojson.Add("TerX", terminalPos.x);
	ojson.Add("TerY", terminalPos.y);
	SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());

}

void Target::attackEnd(Node* sender)
{
	Target* target = dynamic_cast<Target*>(sender);
	m_isAttacking = false;
}

void Target::fireSpiritualPower(Vec2 startPos,Vec2 terminalPos)
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	UILayer::getInstance()->instructorGivesInstruction("Spiritual power incoming!");
	
	bulletLayer->addSpiritualPower(this,startPos,terminalPos);
}

void Target::fireFlameCircle(Vec2 startPos, Vec2 terminalPos)
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	UILayer::getInstance()->instructorGivesInstruction("Watch out\nthat flame circle burns");
	bulletLayer->addFlameCircle(this, startPos, terminalPos);
}

void Target::fireSubterrainAssualt(Vec2 startPos, Vec2 terminalPos)
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();
	UILayer::getInstance()->instructorGivesInstruction("Subterrain Assualt!");
	bulletLayer->addSubterrainAssualt(this, startPos, terminalPos);
}

void Target::doFastStrike(Vec2 startPos, Vec2 terminalPos)
{
	BulletLayer* bulletLayer = BulletLayer::getInstance();

	UILayer::getInstance()->instructorGivesInstruction("!!!!");

	bulletLayer->addFastStrike(this, startPos, terminalPos);
}