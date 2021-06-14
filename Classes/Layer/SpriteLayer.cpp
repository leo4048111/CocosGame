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

	_aiCount = 0;
	addPlayer();

	if(Specs::getInstance()->isSinglePlayer()||(!Specs::getInstance()->isSinglePlayer()&&Specs::getInstance()->isServer()))
	initObstacles();

	this->setName("SpriteLayer");

	if (Specs::getInstance()->isSinglePlayer())
		return true;

	if (Specs::getInstance()->isServer())
		SocketServer::getInstance()->onRecv = CC_CALLBACK_3(SpriteLayer::onRecvServer, this);
	else
		SocketClient::getInstance()->onRecv = CC_CALLBACK_2(SpriteLayer::onRecvClient, this);

	return true;
}

void SpriteLayer::update(float delta)
{
	int maxPlayerCount = Specs::getInstance()->getMaxPlayer();

	if (!Specs::getInstance()->isServer())
		return;

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

void SpriteLayer::initObstacles()
{
	std::vector<std::string> paths;
	Specs::getInstance()->getAllFiles("objects/obstacle", paths, "png");

	for (int c = 0; c < 80; c++)
	{
		auto ob = Entity::createEntity();
		std::string str = paths[random() % paths.size()];
		auto sprite = Sprite::create("objects/obstacle/" + str);
		ob->bindSprite(sprite);
		ob->setPosition(Vec2(Vec2(random() % (int)(MAP_RIGHT_BORDER - MAP_LEFT_BORDER) + MAP_LEFT_BORDER, random() % (int)(MAP_TOP_BORDER - MAP_BOTTOM_BORDER) + MAP_BOTTOM_BORDER)));
		this->addChild(ob,1000);
		ob->setScale(0.3f);
		m_obstacles.push_back(ob);
		ob->setName(str);

		if (Specs::getInstance()->isSinglePlayer())
			continue;

		neb::CJsonObject ojson;
		ojson.Add("Type", JsonMsgType::AddOb);
		ojson.Add("What", str);
		ojson.Add("PosX", ob->getPosition().x);
		ojson.Add("PosY", ob->getPosition().y);
		SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());
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
			std::string name =Specs::getInstance()->m_allPlayerSocket[client];
			auto player = Player::createPlayer(name);
			this->addChild(player, 20, name);
			m_players.push_back(player);
			player->setPosition(dst);
			player->scheduleUpdate();

			_socketLock.lock();
			m_playerSocketMap.insert(std::make_pair(client,player)); //bind a player to each client
			_socketLock.unlock();
		}
	}
	else //init as client
	{
		for (auto name:Specs::getInstance()->m_allPlayerName)
		{
			if (name == Specs::getInstance()->getPlayerName())
				continue;
			auto player = Player::createPlayer(name);
			this->addChild(player, 20, name);
			m_players.push_back(player);
			player->setPosition(dst);
			player->scheduleUpdate();
		}
	}

}

void SpriteLayer::addAiPlayer()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vec2 dst = Vec2(Vec2(random() % (int)(MAP_RIGHT_BORDER - MAP_LEFT_BORDER) + MAP_LEFT_BORDER, random() % (int)(MAP_TOP_BORDER - MAP_BOTTOM_BORDER) + MAP_BOTTOM_BORDER));

	//init ai player
	auto aiPlayer = AiPlayer::createPlayer("AI_" + Value(_aiCount).asString());

	this->addChild(aiPlayer, 20, _aiCount);
	_aiCount++;
	aiPlayer->setPosition(dst);
	aiPlayer->scheduleUpdate();

	m_players.push_back(aiPlayer);

	if (Specs::getInstance()->isSinglePlayer())
		return;

	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::AddAi);
	ojson.Add("Weapon", aiPlayer->getCurrentWeapon()->getWeaponType());
	ojson.Add("PosX", aiPlayer->getPosition().x);
	ojson.Add("PosY", aiPlayer->getPosition().y);
	ojson.Add("Tag", aiPlayer->getName());
	SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());
}

void SpriteLayer::addAiPlayer(double posX, double posY, weaponType weapon, std::string name)
{
	auto aiPlayer = AiPlayer::createPlayer(name);
	this->addChild(aiPlayer, 20, name);
	_aiCount++;
	aiPlayer->setPosition(Vec2(posX,posY));
	aiPlayer->setWeapon(weapon);
	m_players.push_back(aiPlayer);
}

void SpriteLayer::addTarget()
{
	auto target = Target::createTarget();
	target->showStaminaBar();
	this->addChild(target, 20);
	target->setPosition(Vec2(random() % (int)(MAP_RIGHT_BORDER-MAP_LEFT_BORDER)+ MAP_LEFT_BORDER, random() % (int)(MAP_TOP_BORDER - MAP_BOTTOM_BORDER) + MAP_BOTTOM_BORDER));

	if (Specs::getInstance()->isSinglePlayer())
		return;

	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::AddTarget);
	ojson.Add("What", target->getTargetType());
	ojson.Add("PosX", target->getPosition().x);
	ojson.Add("PosY", target->getPosition().y);
	ojson.Add("Tag", target->getTag());
	SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());

	m_targets.pushBack(target);
	target->scheduleUpdate();
}

void SpriteLayer::addTarget(double posX,double posY,targetType type,int tag)
{
	auto target = Target::createTarget();
	target->setTargetType(type);
	target->showHealthBar();
	target->showStaminaBar();
	this->addChild(target, 20);
	target->setPosition(Vec2(posX,posY));
	target->setTag(tag);
	target->setTargetType((targetType)(random() % 4));

	m_targets.pushBack(target);

	//dont update 
}

Vector<Target*> SpriteLayer::getAllTargets()
{
	return m_targets;
}

std::vector<Entity*> SpriteLayer::getAllPlayers()
{
	return m_players;
}

std::vector<Entity*> SpriteLayer::getObstacles()
{
	return m_obstacles;
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

void SpriteLayer::onRecvServer(HSocket socket, const char* data, int count)
{
	neb::CJsonObject ojson(data);
	if (!ojson.Parse(data))
		return;

	int type = 0;
	if (!ojson.Get("Type", type))
		return;

	switch (type)
	{
	case JsonMsgType::PlayerData:
		_socketLock.lock();
		if (m_playerSocketMap[socket] != NULL)
			m_playerSocketMap[socket]->updateWithSyncData(ojson);
		_socketLock.unlock();

		ojson.Add("Tag", Specs::getInstance()->m_allPlayerSocket[socket]); //broadcast the message to each socket
		SocketServer::getInstance()->castMessage(ojson.ToString().c_str(), ojson.ToString().length(), socket);
		break;
	case JsonMsgType::Speak:
	{
		std::string str;
		ojson.Get("Sentence", str);
		m_playerSocketMap[socket]->speak(str);
		Chatbox::getInstance()->appendTextInChat(str);
	}
	break;
	default:
		break;
	}
}

void SpriteLayer::onRecvClient(const char* data, int count)
{
	neb::CJsonObject ojson(data);
	//log(data);
	if (!ojson.Parse(data))
		return;

	int type = 0;
	if (!ojson.Get("Type", type))
		return;

	switch (type)
	{
	case JsonMsgType::PlayerData:
	{
		_socketLock.lock();
		std::string tag;
		ojson.Get("Tag", tag);
		if (tag == Specs::getInstance()->getPlayerName())
			break;

		Player* player = dynamic_cast<Player*>(this->getChildByName(tag));
		if (player != NULL)
			player->updateWithSyncData(ojson);
		_socketLock.unlock();
	}
		break;

	case JsonMsgType::Speak:
	{
		std::string str;
		ojson.Get("Sentence", str);
		std::string tag;
		ojson.Get("Tag", tag);
		if (tag == Specs::getInstance()->getPlayerName())
			break;
		Player* player = dynamic_cast<Player*>(this->getChildByName(tag));
		if (player != NULL)
			player->speak(str);
		Chatbox::getInstance()->appendTextInChat(str);
	}
	break;

	case JsonMsgType::AddTarget:
	{
		int tgType;
		ojson.Get("what", tgType);
		double PosX, PosY;
		ojson.Get("PosX", PosX); 
		ojson.Get("PosY", PosY);
		int tag;
		ojson.Get("Tag", tag);
		addTarget(PosX, PosY, (targetType)tgType, tag);
	}
	break;
	case JsonMsgType::TargetData:
	{
		double PosX, PosY;
		ojson.Get("PosX", PosX);
		ojson.Get("PosY", PosY);
		int dir;
		ojson.Get("Dir", dir);
		int tag;
		ojson.Get("Tag", tag);
		auto target = dynamic_cast<Target*>(this->getChildByTag(tag));
		if(target!=NULL)
		target->moveWithPos(PosX, PosY, dir);
	}
	break;
	case JsonMsgType::TargetAttack:
	{
		double PosX, PosY;
		ojson.Get("TerX", PosX);
		ojson.Get("TerY", PosY);
		int tag;
		ojson.Get("Tag", tag);
		auto target = dynamic_cast<Target*>(this->getChildByTag(tag));
		if (target != NULL)
		{
			target->attack(target->getPosition(), Vec2(PosX, PosY));
		}
	}
	break;
	case JsonMsgType::AddOb:
	{
		double PosX, PosY;
		ojson.Get("PosX", PosX);
		ojson.Get("PosY", PosY);
		std::string str;
		ojson.Get("What", str);
		auto ob = Entity::create();
		auto sprite = Sprite::create("objects/obstacle/" + str);
		ob->bindSprite(sprite);
		ob->setScale(0.3f);
		this->addChild(ob,10000);
		ob->setPosition(Vec2(PosX, PosY));
		m_obstacles.push_back(ob);
	}
	break;
	case JsonMsgType::AddAi:
	{
		double PosX, PosY;
		ojson.Get("PosX", PosX);
		ojson.Get("PosY", PosY);
		int wType;
		ojson.Get("Weapon", wType);
		std::string name;
		ojson.Get("Tag", name);
		addAiPlayer(PosX, PosY, (weaponType)wType, name);
	}
	break;
	default:
		break;
	}
}