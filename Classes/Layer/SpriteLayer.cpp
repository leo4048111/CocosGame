#include "SpriteLayer.h"
#include "UILayer.h"
#include "Controls/Specs.h"
#include "CJsonObject/CJsonObject.hpp"

USING_NS_CC;

SpriteLayer* SpriteLayer::_instance = NULL;

SpriteLayer* SpriteLayer::createSpriteLayer()
{
	if (_instance == NULL)
		_instance = SpriteLayer::create();
	return _instance;
}

bool SpriteLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	//init target specs
	this->initTargetSpecs();

	//init collectable layer
	auto collectableLayer = CollectableLayer::getInstance();
	this->addChild(collectableLayer,20,"CollectableLayer");
	collectableLayer->scheduleUpdate();

	addPlayer();

	this->setName("SpriteLayer");

	if (Specs::getInstance()->isSinglePlayer())
		return true;

	if(Specs::getInstance()->isServer())
	SocketServer::getInstance()->onRecv = CC_CALLBACK_3(SpriteLayer::onRecv, this);

	return true;
}

void SpriteLayer::update(float delta)
{
	int maxPlayerCount = Specs::getInstance()->getMaxPlayer();

	if (m_players.size()< Specs::getInstance()->getMaxPlayer())
	{
		addAiPlayer();
	}

	//update target
	if (m_targets.size() < MIN_TARGETS_COUNT+Specs::getInstance()->getCurrentRound())
	{
		addTarget();
	}
}

void SpriteLayer::addPlayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vec2 dst = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 3);

	//init player
	m_mainPlayer = Player::createPlayer(Specs::getInstance()->getPlayerName());
	m_mainPlayer->setMe(true);
	this->addChild(m_mainPlayer, 20, Specs::getInstance()->getPlayerName());
	m_players.push_back(m_mainPlayer);
	m_mainPlayer->setPosition(dst);
	m_mainPlayer->setControlOnListen();
	m_mainPlayer->scheduleUpdate();

	if (Specs::getInstance()->isSinglePlayer())
		return;

	if (Specs::getInstance()->isServer()) //init as server
	{
		auto clientSockets = SocketServer::getInstance()->getClientSockets();
		for (auto client : clientSockets)
		{
			auto player = Player::createPlayer("test");
			player->setMe(false);
			this->addChild(player, 20, "test");
			m_players.push_back(player);
			player->setPosition(dst);
			player->scheduleUpdate();

			_socketLock.lock();
			m_playerSocketMap.insert(std::make_pair(client,player)); //bind a player to each client
			_socketLock.unlock();
		}

	}

}

void SpriteLayer::addAiPlayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vec2 dst = Vec2(random() % ((int)Director::getInstance()->getVisibleSize().width), random() % ((int)Director::getInstance()->getVisibleSize().height));

	//init ai player
	auto aiPlayer = AiPlayer::createPlayer("AI_" + Value(m_players.size()).asString());
	this->addChild(aiPlayer, 20, "AI_" + Value(m_players.size()).asString());
	aiPlayer->setPosition(dst);
	aiPlayer->scheduleUpdate();

	m_players.push_back(aiPlayer);
}

void SpriteLayer::addTarget()
{
	auto target = Target::createTarget();
	target->showStaminaBar();
	this->addChild(target, 20);
	target->setPosition(Vec2(random() % ((int)Director::getInstance()->getVisibleSize().width), random() % ((int)Director::getInstance()->getVisibleSize().height)));
	m_targets.pushBack(target);
	target->scheduleUpdate();
}

Vector<Target*> SpriteLayer::getAllTargets()
{
	return m_targets;
}

std::vector<Entity*> SpriteLayer::getAllPlayers()
{
	return m_players;
}

void SpriteLayer::initTargetSpecs()
{
	//init target score map
	m_targetScoreMap.insert(std::make_pair(targetType::ghost, 10));
	m_targetScoreMap.insert(std::make_pair(targetType::jellyGhost, 20));
	m_targetScoreMap.insert(std::make_pair(targetType::sadGhost, 5));
	m_targetScoreMap.insert(std::make_pair(targetType::spirit, 30));

}

int SpriteLayer::getThisTargetScore(Target* target)
{
	return m_targetScoreMap[target->getTargetType()];
}

void SpriteLayer::removePlayer(Entity* player)
{
	for (vector<Entity*>::iterator iter = m_players.begin(); iter != m_players.end(); iter++)
	{       
		if (*iter == player)
		{
			m_players.erase(iter);
			break;
		}
	}
}

void SpriteLayer::removeTarget(Target* target)
{
	m_targets.eraseObject(target);
}

void SpriteLayer::onRecv(HSocket socket, const char* data, int count)
{
	char* tmp = const_cast<char*>(data);
	tmp[count] = '\0';

	neb::CJsonObject ojson(tmp);
	if (!ojson.Parse(tmp))
		return;

	int type = 0;
	if (!ojson.Get("Type", type))
		return;
	switch (type)
	{
	case JsonMsgType::PlayerData:
		if (Specs::getInstance()->isServer()) //onRecv as server
		{
			_socketLock.lock();
			if (m_playerSocketMap[socket] != NULL)
				m_playerSocketMap[socket]->updateWithSyncData(ojson);
			_socketLock.unlock();

			ojson.Add("Tag", socket); //broadcast the message to each socket
			return;
		}

		//onRecv as client

	default:
		break;
	}
	

}