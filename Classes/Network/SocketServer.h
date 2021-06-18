#pragma once
#include "SocketBase.h"

class SocketServer : public SocketBase
{
	struct RecvData
	{
		HSocket socketClient;
		int dataLen;
		char data[1024];
	};

private:
	static SocketServer* _instance;

	static SocketServer* create();

	SocketServer();
	~SocketServer();

public:
	static SocketServer* getInstance() {
		if (_instance == NULL)
			_instance = SocketServer::create();
		return _instance;
	}

	void close();

	bool startServer();
	void sendMessage(HSocket socket, const char* data, int count);
	void sendMessage(const char* data, int count);
	void castMessage(const char* data, int count, HSocket socket);
	void destroyInstance();

	std::function<void(const char* ip)> onStart;
	std::function<void(HSocket socket)> onNewConnection;
	std::function<void(HSocket socket, const char* data, int count)> onRecv;
	std::function<void(HSocket socket)> onDisconnect;

	void update(float delta);

	std::list<HSocket> getClientSockets();

private:
	bool initServer();
	void acceptClient();
	void acceptFunc();
	void newClientConnected(HSocket socket);
	void recvMessage(HSocket socket);

private:
	HSocket _socketServer;

private:
	std::list<HSocket> _clientSockets;
	std::list<SocketMessage*> _UIMessageQueue;  // contains all messages
	std::mutex   _UIMessageQueueMutex;      
	std::mutex _sendMessageLock;


};

