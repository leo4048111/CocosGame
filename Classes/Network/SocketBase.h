#pragma once

#include "cocos2d.h"
#include <list>
#include <thread>
USING_NS_CC;
// for windows platform
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET
// for android platform
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <arpa/inet.h>	// for inet_**
#include <netdb.h>	// for gethost**
#include <netinet/in.h>	// for sockaddr_in
#include <sys/types.h>	// for socket
#include <sys/socket.h>	// for socket
#include <unistd.h>
#include <stdio.h>	     // for printf
#include <stdlib.h>	// for exit
#include <string.h>	// for bzero
#define HSocket int
#endif 

class SocketBase : public Ref
{
public:
	SocketBase();
	~SocketBase();

protected:
	void closeConnect(HSocket socket);
	bool error(HSocket socket);

protected:
	std::mutex _mutex;

private:
	bool _bInitSuccess;
};

enum MessageType
{
	DISCONNECT,
	RECEIVE,
	NEW_CONNECTION
};

class SocketMessage
{
private:
	MessageType msgType;    //message type
	Data* msgData;          //message data
public:
	SocketMessage(MessageType type, unsigned char* data, int dataLen)
	{
		msgType = type;
		msgData = new Data;
		msgData->copy(data, dataLen);
	}

	SocketMessage(MessageType type)
	{
		msgType = type;
		msgData = nullptr;
	}

	Data* getMsgData() { return msgData; }
	MessageType getMsgType() { return msgType; }

	~SocketMessage()
	{
		if (msgData)
			CC_SAFE_DELETE(msgData);
	}
};