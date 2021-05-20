#ifndef _UI_LAYER_H_
#define _UI_LAYER_H_

#include "cocos2d.h"
#include "Objects/CrossHair.h"
#include "Scene/GameoverScene.h"

#define SCORE_PER_ROUND 200

class UILayer :public cocos2d::Layer
{
public:
	static UILayer* createUILayer();

	virtual bool init();

	CREATE_FUNC(UILayer);

	virtual void update(float delta);

	void addScore(int score);

	int getScore();

	int getCurrentRound();

	void goToGameoverScene();

	void setWinOrLose(bool status);

	bool isWin();

private:
	//Timer specs
	cocos2d::Label* m_labelTimer;

	//mainCharacter spec status
	cocos2d::Label* m_currentSpeedLabel;
	cocos2d::Label* m_currentResistanceLabel;

	//score board
	cocos2d::Label* m_labelScoreBoard;

};

#endif // !_UI_LAYER_H_
