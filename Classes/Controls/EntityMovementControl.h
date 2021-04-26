#ifndef _ENTITY_MOVEMENT_CONTROL_H_
#define _ENTITY_MOVEMENT_CONTROL_H_

#include "cocos2d.h"
#include <map>

class EntityMovementControl:public cocos2d::Node
{

public:

	virtual bool init();

	static EntityMovementControl* createEntityControl();

	CREATE_FUNC(EntityMovementControl);

	//override function onKeyPressed
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//override function onKeyReleased
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	//set control on listen
	void setControlOnListen();

	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
};




#endif // !_ENTITY_MOVEMENT_H_
