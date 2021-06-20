#pragma once

#include "cocos2d.h"
#include <vector>

class MiniMap :public cocos2d::Node
{
private:
	static MiniMap* _instance;

	MiniMap() {};

	~MiniMap() { this->removeFromParent();  _instance = NULL; };
public:

	static MiniMap* createMiniMap();

	virtual bool init();

	CREATE_FUNC(MiniMap);

	static MiniMap* getInstance() {
		if (_instance == NULL)
			_instance = createMiniMap();
		return _instance;
	}

	void UpdateMe(cocos2d::Vec2 newPos);

	void addTarget(int tag,cocos2d::Vec2 Pos);

	void updateTarget(int tag, cocos2d::Vec2 newPos);

	void removeTarget(int tag);

	void addAi(std::string name, cocos2d::Vec2 Pos);

	void updateAi(std::string name, cocos2d::Vec2 newPos);

	void removeAi(std::string name);

private:

	cocos2d::TMXTiledMap* _map;
	cocos2d::Sprite* _Me;
};


