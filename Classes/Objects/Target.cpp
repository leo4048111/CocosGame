#include "Target.h"

USING_NS_CC;


Target* Target::createTarget()
{
	return Target::create();
}

bool Target::init()
{

	if (!Entity::init())
	{
		return false;
	}

	auto target = Sprite::create("objects/target/target_test1.png");
	target->setScale(0.2f);
	Target::bindSprite(target);
	Target::showHealthBar();

	return true;
}

void Target::update(float delta)
{
	double offset = 2.0f;
	auto mainCharacter=this->getParent()->getChildByName("MainCharacter");
	Vec2 mainCharacterPos = mainCharacter->getParent()->convertToWorldSpaceAR(mainCharacter->getPosition());
	Vec2 targetPos = this->getParent()->convertToWorldSpaceAR(this->getPosition());
	Vec2 dst = mainCharacterPos - targetPos;
	double radians = atan2(dst.y, dst.x);
	double degree = CC_RADIANS_TO_DEGREES(radians);
	double offsetX = offset * cos(degree);
	double offsetY = offset * sin(degree);
	this->setPosition(this->getPosition().x - offsetX, this->getPosition().y-offsetY);
}
