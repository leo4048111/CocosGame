#pragma once

#include "cocos2d.h"

class CreateRoomScene :public cocos2d::Scene
{
public:
	static CreateRoomScene* createCreateRoomScene();

	virtual bool init();

	CREATE_FUNC(CreateRoomScene);

	void menuCallBack(Ref* sender);

	void goToSetupAndGTGScene();

	void backToPreparationScene();

	void addMaxPlayer();

	void minusMaxPlayer();


private:
	cocos2d::Label* m_word;

	int m_maxPlayer;

	cocos2d::Label* m_maxPlayerDisplay;
};