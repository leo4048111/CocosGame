#ifndef _TARGET_LAYER_H_
#define _TARGET_LAYER_H_

#include "cocos2d.h"
#include "Objects/Target.h"
#include "Objects/Player.h"
#include "Objects/AiPlayer.h"
#include "CollectableLayer.h"
#include "Network/SocketServer.h"
#include "Network/SocketClient.h"
#include "Objects/Entity.h"
#include <map>

#define MIN_TARGETS_COUNT 6


class SpriteLayer :public cocos2d::Layer
{
private:
	SpriteLayer() {};

	~SpriteLayer() { this->removeFromParentAndCleanup(1);  _instance = NULL; };

	static SpriteLayer* _instance;

	static SpriteLayer* createSpriteLayer();

	virtual bool init();

public:
	static SpriteLayer* getInstance() {
		if (_instance == NULL)
			_instance = createSpriteLayer();
		return _instance;
	}

public:
	CREATE_FUNC(SpriteLayer);

	virtual void update(float delta);

	cocos2d::Vector<Target*> getAllTargets();

	std::vector<Entity*> getAllPlayers();

	std::vector <std::string>getAllPlayerNames();

	std::vector<Entity*> getObstacles();

	//init obstacles
	void initObstacles();

	//target specs related
	void initTargetSpecs();

	int getThisTargetScore(Target* target);

	void addPlayer();

	void addAiPlayer();

	void addAiPlayer(double posX, double posY, weaponType weapon, std::string name,int tag);

	void addTarget();

	void addTarget(double posX, double posY, targetType type, int tag);

	void removePlayer(Entity* player);

	void removeTarget(Target* target);

	//socket related
	void onRecvServer(HSocket socket, const char* data, int count); //SERVER

	void onRecvClient(const char* data, int count); //client
private:

	//Target specs
	cocos2d::Vector<Target*> m_targets;
	int m_targetCount;
	std::map<targetType, int> m_targetScoreMap;

	//Player specs
	Player* m_mainPlayer;
	std::vector<Entity*> m_players;

	//obstacles
	std::vector<Entity*> m_obstacles;

	//socket specs
	std::map<SOCKET, Player*> m_playerSocketMap;

	//update lock
	std::mutex _socketLock;

	//aiplayer count
	int _aiCount;

};

#endif // !_TARGET_LAYER_H_
