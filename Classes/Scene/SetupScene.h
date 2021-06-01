#pragma once

#include "cocos2d.h"

class SetupScene :public cocos2d::Scene
{
public:
	static SetupScene* createSetupScene();

	virtual bool init();

	CREATE_FUNC(SetupScene);

	//menu callback method
	void menuCallBack(Ref* sender);

	void goToPreparationScene();

	void backToMenuScene();

private:
	cocos2d::TextFieldTTF* m_inputNameBox;

	cocos2d::Label* m_word;
};