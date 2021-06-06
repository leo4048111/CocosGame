#include "CreateRoomScene.h"
#include "PreparationScene.h"
#include "GameScene.h"
#include "Controls/Specs.h"
#include "CJsonObject/CJsonObject.hpp"

USING_NS_CC;

CreateRoomScene* CreateRoomScene::createCreateRoomScene()
{
	return CreateRoomScene::create();
}

bool CreateRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();


	auto confirmLabel = Label::createWithTTF("Let's roll", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto confirm = MenuItemLabel::create(confirmLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	confirm->setTag(0);
	confirm->setColor(Color3B(220, 220, 220));
	confirm->setScale(0.3f);

	back->setTag(1);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(confirm, back, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu, 20);
	menu->setName("menu");
	menu->setPositionY(menu->getPositionY() - 50);

	m_maxPlayer = 2;
	m_maxPlayerDisplay = Label::createWithTTF("Max Player:" + Value(m_maxPlayer).asString(), "fonts/HashedBrowns-WyJgn.ttf", 20);
	this->addChild(m_maxPlayerDisplay);
	m_maxPlayerDisplay->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 15));

	auto addButtonLabel = Label::createWithTTF("+", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto addButton = MenuItemLabel::create(addButtonLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	addButton->setTag(2);
	auto minusButtonLabel = Label::createWithTTF("-", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto minusButton = MenuItemLabel::create(minusButtonLabel, CC_CALLBACK_1(CreateRoomScene::menuCallBack, this));
	minusButton->setTag(3);
	auto menu2 = Menu::create(addButton, minusButton, NULL);
	this->addChild(menu2);
	menu2->alignItemsHorizontallyWithPadding(10);

	createRoom();

	return true;
}

void CreateRoomScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(CreateRoomScene::startGame, this));
	FiniteTimeAction* action = Sequence::create(DelayTime::create(1.5f), callFunc, NULL);

	switch (item->getTag())
	{
	case 0:
		this->runAction(action);
		break;
	case 1:
		backToPreparationScene();
		break;
	case 2:
		addMaxPlayer();
		break;
	case 3:
		minusMaxPlayer();
		break;
	default:
		break;
	}
}

void CreateRoomScene::addMaxPlayer()
{
	if(m_maxPlayer<MAX_PLAYER)
	m_maxPlayer++;

	m_maxPlayerDisplay->setString("Player count:"+Value(m_maxPlayer).asString());
}

void CreateRoomScene::minusMaxPlayer()
{
	if (m_maxPlayer > 2)
		m_maxPlayer--;

	m_maxPlayerDisplay->setString("Player count:"+Value(m_maxPlayer).asString());
}

void CreateRoomScene::startGame()
{
	neb::CJsonObject ojson;
	ojson.Add("Type",JsonMsgType::PlayerList);
	ojson.AddEmptySubArray("Player");
	for (auto name : Specs::getInstance()->m_allPlayerName)
	{
		ojson["Player"].Add(name);
	}
	ojson["Player"].Add(Specs::getInstance()->getPlayerName());
	SocketServer::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());

	neb::CJsonObject ojson2;
	ojson2.Add("Type", JsonMsgType::SCommand);
	ojson2.Add("Cmd", SocketCommand::START);
	SocketServer::getInstance()->sendMessage(ojson2.ToString().c_str(), ojson2.ToString().length());
	Specs::getInstance()->setMaxPlayer(m_maxPlayer);

	auto gameScene = GameScene::createGameScene();
	auto transition = TransitionFlipX::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}

void CreateRoomScene::backToPreparationScene()
{
	auto preparationScene = PreparationScene::createPreparationScene();
	auto transition = TransitionFlipX::create(1.0f, preparationScene);
	Director::getInstance()->replaceScene(transition);
}

void CreateRoomScene::createRoom()
{
	SocketServer::getInstance()->startServer(); //init server
	SocketServer::getInstance()->onRecv = CC_CALLBACK_3(CreateRoomScene::onRecv, this);
	Specs::getInstance()->asServer(true);
	SocketServer::getInstance()->onNewConnection = CC_CALLBACK_1(CreateRoomScene::onNewConnection, this);
	Director::getInstance()->getScheduler()->scheduleUpdate(SocketServer::getInstance(), 0, false); //add server to scheduler, ready for accepting messages
}

void CreateRoomScene::onNewConnection(HSocket socket)
{
	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::SCommand);
	if (m_maxPlayer >= MAX_PLAYER)
	{
		ojson.Add("Cmd", SocketCommand::CANT_JOIN);
		SocketServer::getInstance()->sendMessage(socket, ojson.ToString().c_str(), ojson.ToString().length());
		return;
	}

	ojson.Add("Cmd", SocketCommand::IS_JOIN);
	SocketServer::getInstance()->sendMessage(socket, ojson.ToString().c_str(), ojson.ToString().length());
	m_maxPlayer++;
	m_maxPlayerDisplay->setString("JOINED!");

	//send current player count
	neb::CJsonObject ojson2;
	ojson2.Add("Type", JsonMsgType::PlayerCount);
	ojson2.Add("Num", m_maxPlayer);
	SocketServer::getInstance()->sendMessage(socket, ojson2.ToString().c_str(), ojson2.ToString().length());
}

void CreateRoomScene::onDisconnect(HSocket socket)
{
	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::SCommand);
	ojson.Add("Cmd", SocketCommand::DISCON);
	SocketServer::getInstance()->sendMessage(socket, ojson.ToString().c_str(), ojson.ToString().length());
}

void CreateRoomScene::onRecv(HSocket socket, const char* data, int count)
{
	neb::CJsonObject ojson(data);
	if (!ojson.Parse(data))
		return;

	int type = 0;
	if (!ojson.Get("Type", type))
		return;

	if (type == JsonMsgType::PlayerName)
	{
		std::string name;
		ojson.Get("Name", name);
		Specs::getInstance()->m_allPlayerSocket.insert(make_pair(socket,name));
		Specs::getInstance()->m_allPlayerName.push_back(name);

	}

}