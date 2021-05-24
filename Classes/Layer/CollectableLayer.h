#ifndef _COLLECTABLE_LAYER_H_
#define _COLLECTABLE_LAYER_H_

#include "cocos2d.h"
#include "Objects/Collectable.h"
#include "Objects/Player.h"

class CollectableLayer :public cocos2d::Layer
{
private:
	CollectableLayer() {};

	~CollectableLayer() { this->removeFromParentAndCleanup(1);  _instance = NULL; };

	static CollectableLayer* _instance;

	static CollectableLayer* createCollectableLayer();

	virtual bool init();

public:
	static CollectableLayer* getInstance(){
		if (_instance == NULL)
			_instance = createCollectableLayer();
		return _instance;
	}

public:
	CREATE_FUNC(CollectableLayer);

	void addCollectable(Collectable* collectable, float x, float y);

	virtual void update(float delta);

private:
	cocos2d::Vector<Collectable*> m_allCollectables;
};

#endif // !_COLLECTABLE_LAYER_H_
