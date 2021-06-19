#include "SocketBase.h"

SocketBase::SocketBase()
{
	_bInitSuccess = false;
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 0);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0)
	{
		fprintf(stderr, "Initilize Error!\n");
		return;
	}
	_bInitSuccess = true;

}

SocketBase::~SocketBase()
{
	if (_bInitSuccess)
	{
		WSACleanup();
	}
}

void SocketBase::closeConnect(HSocket socket)
{
	closesocket(socket);
}

bool SocketBase::error(HSocket socket)
{
	return socket == SOCKET_ERROR;
}
