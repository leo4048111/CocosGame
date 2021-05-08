#ifndef _MAIN_CHARACTER_H_
#define _MAIN_CHARACTER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Weapon.h"
#include "ui/UIScrollView.h"
#include <map>

#define MAX_WEAPON_CARRY 4

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

	//Weapon related
	void addWeapon(Weapon* weapon);

	void swapWeapon(int num);

	void setControlOnListen();

	bool canCarryMoreWeapons();

	void dropWeapon();

	void pickUpWeapon();
	
	//MainCharacter spec related
	void addScore(int score);
	
	int getScore();
private:

	//Anime frames
	cocos2d::Vector<cocos2d::SpriteFrame*> m_leftWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_rightWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_forwardWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_backWalkAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_standBackAnime;

	//Control specs
	std::map<cocos2d::EventKeyboard::KeyCode, bool> m_keyMap;
	std::map<cocos2d::EventMouse::MouseButton, bool> m_mouseButtonMap;

	//Weapon specs
	std::map<int,Weapon*> m_weaponsMap;
	std::vector<int> m_emptyWeaponSlots;
	Weapon* m_currentWeapon;
	int m_currentWeaponSlot;
	int m_totalWeapons;
	cocos2d::Label* m_magazineSpecLabel;
	cocos2d::Menu* m_weaponSpecMenu;

	//Score retainer
	int m_currentScore;

};






#endif // !_MAIN_CHARACTER_H_
