#ifndef _UI_LAYER_H_
#define _UI_LAYER_H_

#include "cocos2d.h"
#include "Objects/CrossHair.h"

class UILayer :public cocos2d::Layer
{
public:
	static UILayer* createUILayer();

	virtual bool init();

	CREATE_FUNC(UILayer);

	virtual void update(float delta);

private:
	//Timer specs
	cocos2d::Label* m_labelTimer;
	time_t m_startTime;
	time_t m_endTime;
};

#endif // !_UI_LAYER_H_
