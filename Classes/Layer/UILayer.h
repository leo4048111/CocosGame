#ifndef _UI_LAYER_H_
#define _UI_LAYER_H_

#include "cocos2d.h"
#include "Objects/CrossHair.h"
#include "Objects/Chatbox.h"
#include <vector>

#define SCORE_PER_ROUND 200

class UILayer :public cocos2d::Layer
{
private:
	UILayer() {};

	~UILayer() { this->removeFromParentAndCleanup(1);  _instance = NULL; };

	static UILayer* _instance;

	static UILayer* createUILayer();

	virtual bool init();

public:
	static UILayer* getInstance() {
		if (_instance == NULL)
			_instance = createUILayer();
		return _instance;
	}

public:
	CREATE_FUNC(UILayer);

	virtual void update(float delta);

	//instructor emerges
	void instructorGivesInstruction(std::string str);

	//weapon slot demonstration features
	void unlockSlot(int num);

	void pointToThisSlot(int num);

private:
	//Timer specs
	cocos2d::Label* m_labelTimer;

	//main player spec status
	cocos2d::Label* m_currentSpeedLabel;
	cocos2d::Label* m_currentResistanceLabel;
	cocos2d::Sprite* m_statusWindowBg;
	cocos2d::ProgressTimer* m_playerHp;
	cocos2d::ProgressTimer* m_playerSpeed;
	cocos2d::ProgressTimer* m_playerArmor;

	//score board
	cocos2d::Label* m_labelScoreBoard;

	//weapon status label
	cocos2d::Label* m_weaponLabel;
	std::vector<cocos2d::Label*> m_weaponSlots;
	cocos2d::Sprite* m_pointerArrow;

};

#endif // !_UI_LAYER_H_
