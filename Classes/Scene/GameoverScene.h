#ifndef _GAMEOVER_SCENE_H_
#define _GAMEOVER_SCENE_H_

#include "cocos2d.h"

class GameoverScene :public cocos2d::Scene
{
public:
	static GameoverScene* createGameoverScene();

	virtual bool init();

	CREATE_FUNC(GameoverScene);

	void gameoverCallBack(Ref* sender);

	void backToMenuScene();

};


#endif // !_GAMEOVER_SCENE_H_
