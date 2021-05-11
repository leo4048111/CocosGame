#ifndef _TARGET_LAYER_H_
#define _TARGET_LAYER_H_

#include "cocos2d.h"
#include "Objects/Target.h"
#include "Objects/MainCharacter.h"
#include "CollectableLayer.h"
#include <map>

#define MIN_TARGETS_COUNT 3


class SpriteLayer :public cocos2d::Layer
{
public:
	static SpriteLayer* createSpriteLayer();

	virtual bool init();

	CREATE_FUNC(SpriteLayer);

	virtual void update(float delta);

	cocos2d::Vector<Target*>* getAllTargets();

	//target specs related
	void initTargetSpecs();

	int getThisTargetScore(Target* target);

private:

	//Target specs
	cocos2d::Vector<Target*> m_targets;
	int m_targetCount;
	std::map<targetType, int> m_targetScoreMap;

};

#endif // !_TARGET_LAYER_H_
