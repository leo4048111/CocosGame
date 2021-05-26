#ifndef _TARGET_LAYER_H_
#define _TARGET_LAYER_H_

#include "cocos2d.h"
#include "Objects/Target.h"
#include "Objects/Player.h"
#include "Objects/AiPlayer.h"
#include "CollectableLayer.h"
#include <map>

#define MIN_TARGETS_COUNT 3


class SpriteLayer :public cocos2d::Layer
{
private:
	SpriteLayer() {};

	~SpriteLayer() { this->removeFromParentAndCleanup(1);  _instance = NULL; };

	static SpriteLayer* _instance;

	static SpriteLayer* createSpriteLayer();

	virtual bool init();

public:
	static SpriteLayer* getInstance() {
		if (_instance == NULL)
			_instance = createSpriteLayer();
		return _instance;
	}

public:
	CREATE_FUNC(SpriteLayer);

	virtual void update(float delta);

	cocos2d::Vector<Target*>* getAllTargets();

	//target specs related
	void initTargetSpecs();

	int getThisTargetScore(Target* target);

	void addPlayer();

	void addAiPlayer();

	void addTarget();

	int getPlayerCount();

	int getAiCount();

private:

	//Target specs
	cocos2d::Vector<Target*> m_targets;
	int m_targetCount;
	std::map<targetType, int> m_targetScoreMap;

	//Player specs
	Player* m_mainPlayer;
	cocos2d::Vector<AiPlayer*> m_aiplayers;
};

#endif // !_TARGET_LAYER_H_
