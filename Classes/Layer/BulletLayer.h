#ifndef _BULLET_LAYER_H
#define _BULLET_LAYER_H

#include "cocos2d.h"


#define TARGET_BULLET_SPEED 10.0f

class BulletLayer :public cocos2d::Layer
{
private:
	BulletLayer() {};

	~BulletLayer() { this->removeFromParentAndCleanup(1);  _instance = NULL; };

	static BulletLayer* _instance;

	static BulletLayer* createBulletLayer();

	virtual bool init();

public:
	static BulletLayer* getInstance(){
		if (_instance == NULL)
			_instance = createBulletLayer();
		return _instance;
	}

public:

	CREATE_FUNC(BulletLayer);

	void cleanBullet(Node* sender);

	virtual void update(float delta);

	bool loadGraphs();

	/*friendly shooting specs
	All PNG sources should be placed facing left*/
	void addMeleeAttack();

	void addBullet();

	void addLazer();

	void addSprayBullet();

	void addToxicBomb();

	void addFlameThrower();

	//hostile shooting specs
	void initHostileBulletDamageMap();

	void addSpiritualPower(cocos2d::Node* sender);

	void addFlameCircle(cocos2d::Node* sender);
	
private:
	cocos2d::Vector<cocos2d::Sprite*> m_allFriendlyBullets;
	cocos2d::Vector<cocos2d::Sprite*> m_allHostileBullets;

	std::map < std::string, double > m_hostileBulletDamageMap;

	//anime frames
	cocos2d::Vector<cocos2d::SpriteFrame*> m_explodeAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_chargeSpiritAnime;


};

#endif // !_BULLET_LAYER_H