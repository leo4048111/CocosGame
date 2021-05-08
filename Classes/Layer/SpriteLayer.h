#ifndef _TARGET_LAYER_H_
#define _TARGET_LAYER_H_

#include "cocos2d.h"
#include "Objects/Target.h"
#include "Objects/MainCharacter.h"

#define MIN_TARGETS_COUNT 3

class SpriteLayer :public cocos2d::Layer
{
public:
	static SpriteLayer* createSpriteLayer();

	virtual bool init();

	CREATE_FUNC(SpriteLayer);

	virtual void update(float delta);

	cocos2d::Vector<Target*>* getAllTargets();

private:

	//Target specs
	cocos2d::Vector<Target*> m_targets;
	int m_targetCount;
};

#endif // !_TARGET_LAYER_H_
