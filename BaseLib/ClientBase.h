#pragma once
#ifndef __BASELIB_CLIENT_BASE_H__
#define __BASELIB_CLIENT_BASE_H__

#include "ServerCommon.h"

class BasePacket;

class ClientBase {
public:
	ClientBase() {}
	ClientBase(SOCKET socket) : m_socket(socket) {}
	~ClientBase() {}

	virtual void HandlePacket(BufferInfo* packet) = 0;
	bool SendPacket(BasePacket& packet);
	bool SendPacket(Buffer& buffer);

	SOCKET GetSocket() const { return m_socket; }

protected:
	BufferInfo m_clientBuffer;
	SOCKET m_socket = 0;
	
};

#endif // !__BASELIB_CLIENT_H__
