#ifndef _MAIN_CHARACTER_H_
#define _MAIN_CHARACTER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Weapon.h"
#include <map>
#include <ctime>

#define MAX_WEAPON_CARRY 6
#define SLIDE_COOLDOWN 2.5
enum actions
{
	forward,back,left,right,stand,leftSlide,rightSlide
};

class Player : public Entity
{
public:
	//Sprite related
	static Player* createPlayer();

	virtual bool init();

	bool loadGraphs();

	void runActionAnime(int dir);

	CREATE_FUNC(Player);

	virtual void update(float delta);

	//Control related
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);

	void onMouseUp(cocos2d::Event* event);

	//Weapon related
	void initAllWeapon();

	void addWeapon(Weapon* weapon);

	void swapWeapon(int num);

	void unlockWeapon(int num);

	void fastMeleeAttack();

	void setControlOnListen();

	void setControlOffListen();

	Weapon* getCurrentWeapon();

	std::map<int, Weapon*> getAllWeaponMap();

	//returns sync data as json
	std::string buildSynData();
	

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
	std::map<int,Weapon*> m_allWeaponsMap;
	Weapon* m_currentWeapon;
	int m_currentWeaponSlot;
	int m_lastWeaponSlot;

	cocos2d::Label* m_magazineSpecLabel;
	cocos2d::Menu* m_weaponSpecMenu;

	std::string m_playerName;
};






#endif // !_MAIN_CHARACTER_H_
