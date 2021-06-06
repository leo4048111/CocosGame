#include "SocketClient.h"

SocketClient* SocketClient::_instance = NULL;

SocketClient::SocketClient(void) :
	onRecv(nullptr),
	_socektClient(0)
{
}

SocketClient::~SocketClient(void)
{
	this->clear();
}

bool SocketClient::initClient()
{
	if (_socektClient != 0)
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}

	_socektClient = socket(AF_INET, SOCK_STREAM, 0);
	if (error(_socketServer))
	{
		log("init client error!");
		_socektClient = 0;
		return false;
	}

	return true;
}

bool SocketClient::connectServer(const char* serverIP, unsigned short port)
{
	if (!this->initClient())
	{
		return false;
	}

	struct sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(struct sockaddr_in));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(serverIP);

	int ret = 0;
	ret = connect(_socektClient, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
	if (ret < 0)
	{
		this->closeConnect(_socektClient);
		_socektClient = 0;
		return false;
	}

	std::thread recvThread(&SocketClient::recvMessage, this);
	recvThread.detach();
	return true;
}

void SocketClient::recvMessage()
{
	char recvBuf[1024];
	int ret = 0;
	int dataLen = 0;

	while (true)
	{
		ret = recv(_socektClient, (char*)&dataLen, sizeof(int), 0);
		if (ret < 0)
		{
			log("recv error");
			break;
		}
		if (ret > 0 && onRecv != nullptr)
		{
			ret = recv(_socektClient, recvBuf,dataLen, 0);
			//push data to vector, then parse them in update()
			std::lock_guard<std::mutex> lk(_UIMessageQueueMutex); //set mutex on lock
			recvBuf[ret]='\0';
			SocketMessage* msg = new SocketMessage(RECEIVE,(unsigned char*)recvBuf, ret);
			_UIMessageQueue.push_back(msg);
		}
	}
	_mutex.lock();
	this->closeConnect(_socektClient);
	if (onDisconnect != nullptr)
	{
		onDisconnect();
	}
	_socektClient = 0;
	_mutex.unlock();
}

void SocketClient::sendMessage(const char* data, int count)
{
	_sendMessageLock.lock();
	if (_socektClient != 0)
	{
		int ret = send(_socektClient, (char*)&count, sizeof(int), 0);
		ret = send(_socektClient, data, count, 0);
		if (ret < 0)
		{
			log("send error!");
		}
	}
	_sendMessageLock.unlock();
}

void SocketClient::update(float delta)
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
	case DISCONNECT:
		if (onDisconnect)
			this->onDisconnect();
		break;
	case RECEIVE:
		if (onRecv)
		{
			this->onRecv((const char*)msg->getMsgData()->getBytes(), msg->getMsgData()->getSize());
		}
		break;
	default:
		break;
	}

	CC_SAFE_DELETE(msg); //message cleanup
	_UIMessageQueueMutex.unlock(); 
}


void SocketClient::destroy()
{
	if(this!=nullptr)
	delete this;
}

void SocketClient::clear()
{
	if (_socektClient != 0) //close socket
	{
		_mutex.lock();
		this->closeConnect(_socektClient);
		_mutex.unlock();
	}

	for (auto msg : _UIMessageQueue)    //delete message
	{
		CC_SAFE_DELETE(msg);
	}
	_UIMessageQueue.clear();

	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);

}

void SocketClient::close()
{
	if(_socektClient!=0)
	closeConnect(_socektClient);
}

HSocket SocketClient::getClientSocket()
{
	return _socektClient;
}