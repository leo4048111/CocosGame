#pragma once

#include "cocos2d.h"
#include "Network/SocketServer.h"

class CreateRoomScene :public cocos2d::Scene
{
public:
	static CreateRoomScene* createCreateRoomScene();

	virtual bool init();

	CREATE_FUNC(CreateRoomScene);

	void menuCallBack(Ref* sender);

	void backToPreparationScene();

	void startGame();

	void addMaxPlayer();

	void minusMaxPlayer();

	//create room
	void createRoom();

	void onRecv(HSocket socket, const char* data, int count);

private:
	int m_maxPlayer;

	cocos2d::Label* m_maxPlayerDisplay;

	//server functions in create room
	void onNewConnection(HSocket socket);

	void onDisconnect(HSocket socket);
};