#pragma once

#include "cocos2d.h"

class PreparationScene :public cocos2d::Scene
{
public:
	static PreparationScene* createPreparationScene();

	virtual bool init();

	CREATE_FUNC(PreparationScene);

	//menu callback method
	void menuCallBack(Ref* sender);

	void backToMenuScene();

	void goToSetupAndGTGScene();

};