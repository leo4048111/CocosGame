#include "JoinRoomScene.h"
#include "PreparationScene.h"
#include "GameScene.h"
#include "Controls/Specs.h"
#include "CJsonObject/CJsonObject.hpp"

USING_NS_CC;

JoinRoomScene* JoinRoomScene::createJoinRoomScene()
{
	return JoinRoomScene::create();
}

bool JoinRoomScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	m_inputIP = TextFieldTTF::textFieldWithPlaceHolder("Enter host ip", "Southampton-MRqp.ttf", 15);
	m_inputIP->setTextColor(Color4B(255, 255, 255, 100));
	this->addChild(m_inputIP);
	m_inputIP->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_inputIP->attachWithIME();

	m_connectionStatus = Label::createWithTTF("Disconnected", "fonts/HashedBrowns-WyJgn.ttf", 50);
	this->addChild(m_connectionStatus);
	m_connectionStatus->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 4 / 5));

	m_joinStatus = Label::createWithTTF("Not joined", "fonts/HashedBrowns-WyJgn.ttf", 50);
	this->addChild(m_joinStatus);
	m_joinStatus->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 3 / 5));

	auto connectLabel = Label::createWithTTF("Connect", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto connect = MenuItemLabel::create(connectLabel, CC_CALLBACK_1(JoinRoomScene::menuCallBack, this));
	auto backLabel = Label::createWithTTF("Back", "fonts/HashedBrowns-WyJgn.ttf", 50);
	auto back = MenuItemLabel::create(backLabel, CC_CALLBACK_1(JoinRoomScene::menuCallBack, this));
	connect->setTag(0);
	connect->setColor(Color3B(220, 220, 220));
	connect->setScale(0.3f);
	back->setTag(1);
	back->setColor(Color3B(220, 220, 220));
	back->setScale(0.3f);

	auto menu = Menu::create(connect, back, NULL);
	menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	menu->alignItemsVerticallyWithPadding(15);
	this->addChild(menu, 20);
	menu->setName("menu");
	menu->setPositionY(menu->getPositionY() - 50);

	return true;
}

void JoinRoomScene::menuCallBack(Ref* sender)
{
	MenuItem* item = dynamic_cast<MenuItem*>(sender);
	switch (item->getTag())
	{
	case 0:
		joinRoom();
		break;
	case 1:
		backToPreparationScene();
		break;
	default:
		break;
	}
}

bool JoinRoomScene::joinRoom()
{
	SocketClient::getInstance()->onRecv = CC_CALLBACK_2(JoinRoomScene::onRecv, this);
	Specs::getInstance()->asServer(false);
	Director::getInstance()->getScheduler()->scheduleUpdateForTarget(SocketClient::getInstance(), 0, false);
	std::string ipAdr = m_inputIP->getString();
	if (!SocketClient::getInstance()->connectServer(ipAdr.c_str(), 8000))
	{
		return false;
	}

	//send name
	neb::CJsonObject ojson;
	ojson.Add("Type", JsonMsgType::PlayerName);
	ojson.Add("Name", Specs::getInstance()->getPlayerName());
	SocketClient::getInstance()->sendMessage(ojson.ToString().c_str(), ojson.ToString().length());

	m_connectionStatus->setString("Connected");
	return true;
}

void JoinRoomScene::backToPreparationScene()
{
	SocketClient::getInstance()->close();

	PreparationScene* preparationScene = PreparationScene::create();
	auto transition = TransitionFlipX::create(0.5f, preparationScene);
	Director::getInstance()->replaceScene(transition);
}

void JoinRoomScene::onRecv(const char* data, int count)
{
	neb::CJsonObject ojson(data);
	if (!ojson.Parse(data))
		return;

	int type = 0;
	ojson.Get("Type", type);
	if (type == JsonMsgType::PlayerCount)
	{
		int count = 0;
		ojson.Get("Num", count);
		Specs::getInstance()->setMaxPlayer(count);
		return;
	}

	if (type == JsonMsgType::PlayerList)
	{
		int size = ojson["Player"].GetArraySize();
		for (int c = 0; c < size; c++)
		{
			std::string name;
			ojson["Player"].Get(c, name);
			if(name!=Specs::getInstance()->getPlayerName())
			Specs::getInstance()->m_allPlayerName.push_back(name);
		}
	}

	if (type != JsonMsgType::SCommand)
		return;

	int cmd = 0;
	ojson.Get("Cmd", cmd);
	switch (cmd)
	{
	case SocketCommand::CANT_JOIN:
		m_joinStatus->setString("Can't join");
		Director::getInstance()->getScheduler()->unscheduleAllForTarget(SocketClient::getInstance());
		SocketClient::getInstance()->close();
		return;
	case SocketCommand::IS_JOIN:
		m_joinStatus->setString("Joined Host " + m_inputIP->getString());
		return;
	case SocketCommand::START:
		startGame();
		break;
	default:
		break;
	}
	
}

void JoinRoomScene::startGame()
{
	auto gameScene = GameScene::createGameScene();
	auto transition = TransitionFlipX::create(1.0f, gameScene);
	Director::getInstance()->replaceScene(transition);
}