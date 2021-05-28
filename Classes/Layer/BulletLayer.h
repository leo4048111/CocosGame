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

	//anime features
	bool loadGraphs();

	bool loadAudio();

	void runEffect(cocos2d::Node* sender,cocos2d::Vector<cocos2d::SpriteFrame*> effectFrame);

	void pointBulletTo(cocos2d::Node* obj,cocos2d::Vec2 route, double offset);

	/*friendly shooting specs
	All PNG sources should be placed facing left*/
	void addMeleeAttack(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addBullet(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addLazer(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addSprayBullet(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addToxicBomb(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addFlameThrower(cocos2d::Node* sender,cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	//hostile shooting specs
	void initHostileBulletDamageMap();

	void addSpiritualPower(cocos2d::Node* sender, cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

	void addFlameCircle(cocos2d::Node* sender, cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);
	
	void addSubterrainAssualt(cocos2d::Node* sender, cocos2d::Vec2 startPos, cocos2d::Vec2 terminalPos);

private:
	cocos2d::Vector<cocos2d::Sprite*> m_allFriendlyBullets;
	cocos2d::Vector<cocos2d::Sprite*> m_allHostileBullets;

	std::map < std::string, double > m_bulletDamageMap;

	//anime frames
	cocos2d::Vector<cocos2d::SpriteFrame*> m_explodeAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_chargeSpiritAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_flameCircleAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_subterrainAssualtAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_bombAnime;
	cocos2d::Vector<cocos2d::SpriteFrame*> m_hit1Anime;


};

#endif // !_BULLET_LAYER_H