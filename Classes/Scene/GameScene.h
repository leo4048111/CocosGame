#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "cocos2d.h"
#include "../Objects/MainCharacter.h"
#include "../Layer/UILayer.h"
#include "../Layer/GameSettingsLayer.h"
#include "../Layer//SpriteLayer.h"
#include <ctime>

class GameScene :public cocos2d::Scene
{
public:
	static GameScene* createGameScene();

	virtual bool init();

	CREATE_FUNC(GameScene);

	void setControlOnListen();

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void goToGameSettings();

	void update(float delta);

private:
	//Window specs
	cocos2d::Vec2 m_origin;
	cocos2d::Size m_visibleSize;

};



#endif // !_GAME_SCENE_H_
