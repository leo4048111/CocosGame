#pragma once

#include "cocos2d.h"
#include <list>
#include <thread>
USING_NS_CC;

#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define HSocket SOCKET

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