#pragma once

#include "SocketBase.h"

class SocketClient : public SocketBase
{
private:
	SocketClient(void);
	~SocketClient(void);
	void clear();

public:
	SocketClient* construct();
	void destroy();

	bool connectServer(const char* serverIP, unsigned short port);
	void sendMessage(const char* data, int count);

	std::function<void(const char* data, int count)> onRecv;
	std::function<void()> onDisconnect;

	void update(float delta);

private:
	bool initClient();
	void recvMessage();

private:
	HSocket _socketServer;
	HSocket _socektClient;
	std::list<SocketMessage*> _UIMessageQueue;  //contains all messages
	std::mutex   _UIMessageQueueMutex;      //mutex variable for resolving thread collision
};
