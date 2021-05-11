#ifndef _COLLECTABLE_LAYER_H_
#define _COLLECTABLE_LAYER_H_

#include "cocos2d.h"
#include "Objects/Collectable.h"
#include "Objects/MainCharacter.h"

class CollectableLayer :public cocos2d::Layer
{
public:
	static CollectableLayer* createCollectableLayer();
	virtual bool init();
	CREATE_FUNC(CollectableLayer);

	void addCollectable(Collectable* collectable, float x, float y);

	virtual void update(float delta);

private:
	cocos2d::Vector<Collectable*> m_allCollectables;
};

#endif // !_COLLECTABLE_LAYER_H_
