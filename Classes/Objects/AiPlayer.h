#pragma once

#include "cocos2d.h"
#include "Entity.h"
#include "Weapon.h"
#include "Player.h"
#include <map>
#include <ctime>

#define MAX_WEAPON_CARRY 6
#define SLIDE_COOLDOWN 2.5

class AiPlayer : public Entity
{
public:
	//Sprite related
	static AiPlayer* createPlayer();

	virtual bool init();

	bool loadGraphs();

	void runActionAnime(int dir);

	CREATE_FUNC(AiPlayer);

	virtual void update(float delta);

	void deployTo(cocos2d::Vec2 pos);

	//Weapon related
	void initRandomWeapon();

	Weapon* getCurrentWeapon();

	//ai control features
	void AiControlAutoUpdate();


private:

	//Anime frames
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_forwardWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_backWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_standBackAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftSlideAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightSlideAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_parachuteAnime;

	//Control specs
	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;
	std::map<cocos2d::EventMouse::MouseButton, bool> m_mouseButtonMap;

	//Weapon specs
	Weapon* m_currentWeapon;

	std::string m_playerName;

	//update specs
	time_t m_lastUpdate;
};
