#ifndef _ENTITY_H_
#define _ENTITY_H

#include"cocos2d.h"

class Entity :public cocos2d::Node
{
public:
	cocos2d::Sprite* m_sprite;
//private:
//	cocos2d::C* m_control;
public:
	virtual bool init();

	void bindSprite(cocos2d::Sprite* sprite);

};


#endif