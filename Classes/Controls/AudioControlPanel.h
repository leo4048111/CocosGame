#pragma once

#include "cocos2d.h"
#include "audio/include/SimpleAudioEngine.h"

class AudioControlPanel :public cocos2d::Node
{
public:
	static AudioControlPanel* createAudioControlPanel();

	virtual bool init();

	CREATE_FUNC(AudioControlPanel);

	void menuCallback(Ref* sender);

	void menuToggleCallback(Ref* sender);
private:
	cocos2d::Label* m_songName;
};