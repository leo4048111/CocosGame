#pragma once

#include "cocos2d.h"

class SetupAndGTGScene :public cocos2d::Scene
{
public:
	static SetupAndGTGScene* createSetupAndGTGScene();

	virtual bool init();

	CREATE_FUNC(SetupAndGTGScene);

	//menu callback method
	void menuCallBack(Ref* sender);

	void backToPreparationScene();

	void startGame();

private:
	cocos2d::TextFieldTTF* m_inputNameBox;

	cocos2d::Label* m_word;
};