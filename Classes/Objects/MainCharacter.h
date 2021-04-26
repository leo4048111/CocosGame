#ifndef _MAIN_CHARACTER_H_
#define _MAIN_CHARACTER_H_

#include "cocos2d.h"
#include "Entity.h"
#include <map>

enum actions
{
	forward,back,left,right,standBack,standForward
};

class MainCharacter : public Entity
{
public:
	static MainCharacter* createMainCharacter();

	virtual bool init();

	bool loadAnimes();

	void runAction(int dir);

	CREATE_FUNC(MainCharacter);

	virtual void update(float delta);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void setControlOnListen();
	

private:
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_forwardWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_backWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_standBackAnime;

	std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;

};






#endif // !_MAIN_CHARACTER_H_
