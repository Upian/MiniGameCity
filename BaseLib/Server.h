#pragma once
#ifndef __BASELIB_SERVER_H__
#define __BASELIB_SERVER_H__

#include "ClientBase.h"
#include "PlayerManager.h"

class Server : public ClientBase {
public:
	Server() {}
	Server(SOCKET socket) : ClientBase(socket) {}
	~Server() {}

	void SetServerSocket(SOCKET socket) { m_socket = socket; }

private:
	PlayerManager m_playerManager;
};

#endif // !__BASELIB_SERVER_H__
