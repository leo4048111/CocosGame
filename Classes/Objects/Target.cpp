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
