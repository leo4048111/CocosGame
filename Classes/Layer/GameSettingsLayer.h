#ifndef _GAME_SETTINGS_LAYER_H_
#define _GAME_SETTINGS_LAYER_H_

#include "cocos2d.h"

class GameSettingsLayer :public cocos2d::Layer
{
public:
	static GameSettingsLayer* createGameSettingsLayer();

	virtual bool init();

	void backToGameScene();

	void backToMenuScene();

	void gameSettingsCallBack(Ref* sender);

	CREATE_FUNC(GameSettingsLayer);
};

#endif // !_GAME_SETTINGS_LAYER_H_
