#pragma once
#ifndef __BASELIB_SERVER_H__
#define __BASELIB_SERVER_H__

#include "ClientBase.h"
#include "PlayerManager.h"

class ClntServer : public ClientBase {
public:
	ClntServer() {}
	ClntServer(SOCKET socket) : ClientBase(socket) {}
	~ClntServer() {}

	void SetServerSocket(SOCKET socket) { m_socket = socket; }

private:
};

#endif // !__BASELIB_SERVER_H__
