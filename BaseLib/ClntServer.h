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
	void SetServerAddress(std::string& ipAddress, int port) { m_serverIPAddress = ipAddress; m_portNum = port; }
	void SetServerIpAddress(std::string& ipAddress) { m_serverIPAddress = ipAddress; }
	void SetServerPortNum(int port) { m_portNum = port; }

	std::string& GetServerIpAddress() { return m_serverIPAddress; };
	int GetServerPortNum() const { return m_portNum; };
private:
	std::string m_serverIPAddress;
	int m_portNum = 0;

};

#endif // !__BASELIB_SERVER_H__
