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
	static AiPlayer* createPlayer(std::string name);

	virtual bool init();

	bool loadGraphs();

	CREATE_FUNC(AiPlayer);

	bool canMoveForward();

	bool canMoveBack();

	bool canMoveLeft();

	bool canMoveRight();

	void setWeapon(weaponType type);

	virtual void update(float delta);

	void runActionAnime(int dir);

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

	cocos2d::Vector<cocos2d::SpriteFrame*> m_parachuteAnime;

	//Control specs
	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;
	std::map<cocos2d::EventMouse::MouseButton, bool> m_mouseButtonMap;

	//Weapon specs
	Weapon* m_currentWeapon;

	static std::string m_playerName;
	
	//update specs
	time_t m_lastUpdate;
};
