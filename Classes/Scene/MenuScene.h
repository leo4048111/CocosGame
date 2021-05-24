#ifndef  _MENU_SCENE_H_
#define  _MENU_SCENE_H_

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class MenuScene :public cocos2d::Scene
{
public:
	static MenuScene* createMenuScene();

	virtual bool init();

	void menuCallBack(Ref* ref);

	void setBackgroundMusic();

	void goToPreparationScene();

	CREATE_FUNC(MenuScene);
};

#endif 