#pragma once

#include "cocos2d.h"
#include "network/SocketClient.h"
#include <vector>

class JoinRoomScene :public cocos2d::Scene
{
public:
	static JoinRoomScene* createJoinRoomScene();

	virtual bool init();

	CREATE_FUNC(JoinRoomScene);

	void menuCallBack(Ref* sender);

	void backToPreparationScene();

	void startGame();

	//socket client controls
	bool joinRoom();

	void onRecv(const char* data, int count);
private:
	cocos2d::TextFieldTTF* m_inputIP;

	bool m_isJoin;
	cocos2d::Label* m_connectionStatus;
	cocos2d::Label* m_joinStatus;

	std::vector<HSocket> m_allPlayerSockets;
};