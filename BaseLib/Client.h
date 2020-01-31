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

	void Initialize();
	bool SendPacket(BasePacket& packet);

	SOCKET GetSocket() const { return m_socket; }

protected:
	BufferInfo m_clientBuffer;
	SOCKET m_socket = 0;
	
};

#endif // !__BASELIB_CLIENT_H__
