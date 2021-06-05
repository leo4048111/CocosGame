#include "SocketServer.h"

SocketServer* SocketServer::_instance = NULL;

SocketServer* SocketServer::create()
{
	auto server = new SocketServer;

	return server;
}

SocketServer::SocketServer() :
	_socketServer(0),
	onRecv(nullptr),
	onStart(nullptr),
	onNewConnection(nullptr)
{

}

SocketServer::~SocketServer()
{
	_clientSockets.clear();

	if (_socketServer)
	{
		this->closeConnect(_socketServer);
	}
};

void SocketServer::destroyInstance()
{
	CC_SAFE_DELETE(_instance);
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(_instance);
}

bool SocketServer::startServer()
{
	if (!initServer())
	{
		return false;
	}

	return true;
}

bool SocketServer::initServer()
{
	if (_socketServer != 0)
	{
		this->closeConnect(_socketServer);
	}
	_socketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socketServer))
	{
		log("socket error!");
		_socketServer = 0;
		return false;
	}
	do
	{
		struct sockaddr_in sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));

		sockAddr.sin_family = AF_INET;
		sockAddr.sin_port = htons(8000);
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		int ret = 0;
		ret = bind(_socketServer, (const sockaddr*)&sockAddr, sizeof(sockAddr));
		if (ret < 0)
		{
			log("bind error!");
			break;
		}

		ret = listen(_socketServer, 5);
		if (ret < 0)
		{
			log("listen error!");
			break;
		}
		// start 
		char hostName[256];
		gethostname(hostName, sizeof(hostName));
		struct hostent* hostInfo = gethostbyname(hostName);
		char* ip = inet_ntoa(*(struct in_addr*)*hostInfo->h_addr_list);
		this->acceptClient();

		if (onStart != nullptr)
		{
			log("start server!");
			onStart(ip);
		}

		return true;

	} while (false);

	this->closeConnect(_socketServer);
	_socketServer = 0;
	return false;
}

void SocketServer::acceptClient()
{
	std::thread th(&SocketServer::acceptFunc, this);
	th.detach();
}

void SocketServer::acceptFunc()
{
	int len = sizeof(sockaddr);
	struct sockaddr_in sockAddr;
	while (true)
	{
		HSocket clientSock = accept(_socketServer, (sockaddr*)&sockAddr, &len);
		if (error(clientSock))
		{
			log("accept error!");
			break;
		}

		this->newClientConnected(clientSock);
	}
}

void SocketServer::newClientConnected(HSocket socket)
{
	log("new connect!");

	_clientSockets.push_back(socket);
	std::thread th(&SocketServer::recvMessage, this, socket);
	th.detach();

	if (onNewConnection) //push connection message into vector and parse in update()
	{
		std::lock_guard<std::mutex> lk(_UIMessageQueueMutex);
		SocketMessage* msg = new SocketMessage(NEW_CONNECTION, (unsigned char*)&socket, sizeof(HSocket));
		_UIMessageQueue.push_back(msg);
	}
}

void SocketServer::recvMessage(HSocket socket)
{
	char buff[1024];
	int ret = 0;
	int dataLen = 0;

	while (true)
	{
		ret = recv(socket, (char*)&dataLen, sizeof(int), 0);
		if (ret < 0) //invalid data
		{
			log("recv(%d) error!", socket);
			_mutex.lock();
			this->closeConnect(socket);
			_clientSockets.remove(socket);
			if (onDisconnect != nullptr)
			{
				onDisconnect(socket);
			}
			_mutex.unlock();

			break;
		}
		else
		{
			ret = recv(socket, buff, dataLen, 0);
			buff[ret] = 0;

			log("recv msg : %s", buff);
			if (ret > 0 && onRecv != nullptr)
			{
				std::lock_guard<std::mutex> lk(_UIMessageQueueMutex);
				RecvData recvData;          //store socket data
				recvData.socketClient = socket;
				memcpy(recvData.data, buff, ret);
				recvData.dataLen = ret;
				recvData.data[ret] = 0;
				SocketMessage* msg = new SocketMessage(RECEIVE, (unsigned char*)&recvData, sizeof(RecvData));
				_UIMessageQueue.push_back(msg);
			}
		}
	}
}

void SocketServer::sendMessage(HSocket socket, const char* data, int count)
{
	_sendMessageLock.lock();
	for (auto& sock : _clientSockets)
	{
		if (sock == socket)
		{
			int ret = send(socket, data, count, 0);
			if (ret < 0)
			{
				log("send error!");
			}
			break;
		}
	}
	_sendMessageLock.unlock();
}

void SocketServer::sendMessage(const char* data, int count)
{
	_sendMessageLock.lock();
	for (auto& socket : _clientSockets)
	{
		int ret = send(socket, data, count, 0);
		if (ret < 0)
		{
			log("send error!");
		}
	}
	_sendMessageLock.unlock();
}

void SocketServer::update(float delta)
{
	if (_UIMessageQueue.size() == 0) //return if message is empty
	{
		return;
	}

	_UIMessageQueueMutex.lock(); //set mutex on lock if message is not empty

	//double check
	if (_UIMessageQueue.size() == 0)
	{
		_UIMessageQueueMutex.unlock();
		return;
	}

	SocketMessage* msg = *(_UIMessageQueue.begin());
	_UIMessageQueue.pop_front();

	switch (msg->getMsgType())
	{
	case NEW_CONNECTION:
		if (onNewConnection)
		{
			this->onNewConnection(*(HSocket*)msg->getMsgData()->getBytes());
		}
		break;
	case DISCONNECT:
		if (onDisconnect)
		{
			this->onDisconnect(*(HSocket*)msg->getMsgData()->getBytes());
		}
		break;
	case RECEIVE:
		if (onRecv)
		{
			RecvData* recvData = (RecvData*)msg->getMsgData()->getBytes();
			this->onRecv(recvData->socketClient, (const char*)recvData->data, recvData->dataLen);
		}
		break;
	default:
		break;
	}

	CC_SAFE_DELETE(msg); //message cleanup
	_UIMessageQueueMutex.unlock();  // mutex unlock
}

std::list<HSocket> SocketServer::getClientSockets()
{
	return _clientSockets;
}

void SocketServer::close()
{
	closeConnect(_socketServer);
}