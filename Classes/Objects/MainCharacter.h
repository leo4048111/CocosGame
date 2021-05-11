#ifndef _MAIN_CHARACTER_H_
#define _MAIN_CHARACTER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Weapon.h"
#include "ui/UIScrollView.h"
#include <map>
#include <ctime>

#define MAX_WEAPON_CARRY 4
#define SLIDE_COOLDOWN 2.5
enum actions
{
	forward,back,left,right,standBack,standForward
};

class MainCharacter : public Entity
{
public:
	//Sprite related
	static MainCharacter* createMainCharacter();

	virtual bool init();

	bool loadGraphs();

	void runAction(int dir);

	CREATE_FUNC(MainCharacter);

	virtual void update(float delta);

	//Control related
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);

	void onMouseUp(cocos2d::Event* event);

	void addSpeed(float speed);

	time_t m_lastSlideTime;
	void slide();

	//Weapon related
	void addWeapon(Weapon* weapon);

	void swapWeapon(int num);

	void setControlOnListen();

	bool canCarryMoreWeapons();

	void dropWeapon();

	void pickUpWeapon();

	Weapon* getCurrentWeapon();
	
private:

	//Anime frames
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_forwardWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_backWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_standBackAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftSlideAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightSlideAnime;

	//Control specs
	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;
	std::map<cocos2d::EventMouse::MouseButton, bool> m_mouseButtonMap;
	float m_currentSpeed;
	void slideEnd(cocos2d::Node* sender);

	//Weapon specs
	std::map<int,Weapon*> m_weaponsMap;
	std::vector<int> m_emptyWeaponSlots;
	Weapon* m_currentWeapon;
	int m_currentWeaponSlot;
	int m_totalWeapons;
	cocos2d::Label* m_magazineSpecLabel;
	cocos2d::Menu* m_weaponSpecMenu;

};






#endif // !_MAIN_CHARACTER_H_
