#ifndef _CROSS_HAIR_H_
#define _CROSS_HAIR_H_

#include "cocos2d.h"
#include "Entity.h"


class CrossHair :public Entity
{
public:
	static CrossHair* createCrossHair();

	virtual bool init();

	CREATE_FUNC(CrossHair);

	//control specs
	void onMouseMove(cocos2d::Event* event);

	void setControlOnListen();

	cocos2d::Vec2 getCursorPos();

	//hit animes
	void showHitNotification();

	void unshowHitNotification(Node* node);

private:
	cocos2d::Vector<cocos2d::SpriteFrame*> m_fireAnime;
	cocos2d::Vec2 m_cursorPos;

	cocos2d::Sprite* m_hitNotification;
};




#endif // !_CROSS_HAIR_H_
