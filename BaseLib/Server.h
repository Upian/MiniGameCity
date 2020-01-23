#pragma once
#ifndef __BASELIB_SERVER_H__
#define __BASELIB_SERVER_H__

#include "Client.h"

class Server : public Client {
public:
	Server() {}
	Server(SOCKET socket) : Client(socket) {}
	~Server() {}

	void SetServerType(ServerType type) { m_serverType = type; }
	ServerType GetServerType() const { return m_serverType; }

private:
	ServerType m_serverType = ServerType::serverTypeNone;


};

#endif // !__BASELIB_SERVER_H__
