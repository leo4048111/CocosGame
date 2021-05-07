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
		auto cache = SpriteFrameCache::getInstance();
		m_leftWalkAnime.pushBack(cache->getSpriteFrameByName("target_leftMove"));
		m_rightWalkAnime.pushBack(cache->getSpriteFrameByName("target_rightMove"));
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
	Target::loadGraphs();
	auto target = Sprite::createWithSpriteFrameName("target_leftMove");
	Target::bindSprite(target);
	m_currentDir= moveleft;
	target->setScale(0.2f);
	Target::showHealthBar();
	return true;
}

void Target::update(float delta)
{
	//Target always moves towards main character
	double offset = 0.2f;
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

	this->setPosition(this->getPosition().x + offsetX, this->getPosition().y+offsetY);
}
