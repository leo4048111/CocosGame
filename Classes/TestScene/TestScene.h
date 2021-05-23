#pragma once

#include "cocos2d.h"

class TestScene :public cocos2d::Scene
{
public:
	static TestScene* createTestScene();
	virtual bool init();
	CREATE_FUNC(TestScene);
};