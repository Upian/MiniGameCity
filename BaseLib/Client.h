#pragma once
#ifndef __BASELIB_CLIENT_H__
#define __BASELIB_CLIENT_H__

#include "ServerCommon.h"

class BasePacket;

class Client {
public:
	Client() {}
	Client(SOCKET socket) : m_socket(socket) {}
	~Client() {}

	void SendToClient(BasePacket& packet);
	void SetSocket(SOCKET socket) { m_socket = socket; }
	SOCKET GetSocket() const { return m_socket; }

private:
	SOCKET m_socket = 0;
	SOCKADDR_IN m_address;
};

#endif // !__BASELIB_CLIENT_H__
