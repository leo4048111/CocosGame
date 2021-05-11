#include "Target.h"

USING_NS_CC;

Target* Target::createTarget()
{
	return Target::create();
}

bool Target::loadGraphs()
{
	try
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("objects/target/target.plist");
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

	
	Target::setTargetType(ghost);
	Target::showHealthBar();

	return true;
}

void Target::update(float delta)
{
	time_t currentTime = time(NULL);
	if (currentTime - m_lastUpdateTime < 1)
		return;
	//Target always moves towards main character
	m_lastUpdateTime = currentTime;
	double offset = 10.0f;
	auto mainCharacter=this->getParent()->getChildByName("MainCharacter");
	Vec2 mainCharacterPos = mainCharacter->getParent()->convertToWorldSpaceAR(mainCharacter->getPosition());
	Vec2 targetPos = this->getParent()->convertToWorldSpaceAR(this->getPosition());
	Vec2 dst = mainCharacterPos - targetPos;
	double offsetX = offset * cos(atan2(dst.y, dst.x));
	double offsetY = offset * sin(atan2(dst.y, dst.x));

	if (offsetX < 0 && m_currentDir == moveright)
	{
		auto action =RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_leftWalkAnime,1.0f)));
		this->m_sprite->stopActionByTag(moveright);
		action->setTag(moveleft);
		this->m_sprite->runAction(action);
		m_currentDir = moveleft;
		
	}
	else if (offsetX > 0 && m_currentDir == moveleft)
	{
		auto action = RepeatForever::create(Animate::create(Animation::createWithSpriteFrames(m_rightWalkAnime,1.0f)));
		this->m_sprite->stopActionByTag(moveleft);
		action->setTag(moveright);
		this->m_sprite->runAction(action);
		m_currentDir = moveright;
	}

	this->runAction(MoveTo::create(0.5f,Vec2(this->getPosition().x + offsetX, this->getPosition().y+offsetY)));
}

void Target::setTargetType(targetType type)
{
	m_type = type;
	String targetName,targetLeftMoveFrameName, targetRightMoveFrameName;
	//Name format as such: target_$TYPE_$DirMove
	switch (m_type)
	{
	case targetType::ghost:
		targetName = "target_ghost";
		targetLeftMoveFrameName = "target_ghost_leftMove";
		targetRightMoveFrameName = "target_ghost_rightMove";
		break;

	default:
		return;
	}

	//reset target sprite display
	auto target = Sprite::createWithSpriteFrameName(targetName.getCString());
	if (m_sprite != nullptr)
		m_sprite->removeFromParentAndCleanup(1);
	this->setName(targetName.getCString());
	target->setScale(0.2f);
	Target::bindSprite(target);

	//reset target sprite anime frame cache
	auto cache = SpriteFrameCache::getInstance();

	m_leftWalkAnime.pushBack(cache->getSpriteFrameByName(targetLeftMoveFrameName.getCString()));
	m_rightWalkAnime.pushBack(cache->getSpriteFrameByName(targetRightMoveFrameName.getCString()));

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
	collectableType type = (collectableType)(rand() % 1);
	dropSpecificCollectable(type);
}