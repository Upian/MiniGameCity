#pragma once
#ifndef __BASELIB_BASE_SERVER_HANDLER_H__
#define __BASELIB_BASE_SERVER_HANDLER_H__
#include <thread>
#include "Server.h"

class BaseServerHandler {
public:
	void ConnectToServer(std::string ipAddress, short port);
	virtual void RegisterToServer() = 0;

	void SetConnectionReady() { m_isConnectionReady = true; }
	bool GetConnectionReady() const { return m_isConnectionReady; }

	void SetServerSocket(SOCKET socket) { m_server.SetServerSocket(socket); }
	Server& GetServer() { return m_server; }
	void SendPacketToServer(BasePacket& packet) { m_server.SendPacket(packet); }

	virtual void HandlePacket(Buffer& buffer) = 0;//packet from another server
	void RecvThread();
protected:
	
private:
	std::thread m_recvThread;
	Server m_server;
	bool m_isConnectionReady = false;
};
#endif // !__BASELIB_BASE_SERVER_HANDLER_H__
