#pragma once
#ifndef __BASELIB_SERVER_H__
#define __BASELIB_SERVER_H__

#include "ClientBase.h"

class Server : public ClientBase {
public:
	Server() {}
	Server(SOCKET socket) : ClientBase(socket) {}
	~Server() {}

	void SetServerType(ServerType type) { m_serverType = type; }
	ServerType GetServerType() const { return m_serverType; }

private:
	ServerType m_serverType = ServerType::serverTypeNone;


};

#endif // !__BASELIB_SERVER_H__
