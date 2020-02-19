#pragma once
#ifndef __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
#define __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__

#include "BaseServer.h"
#include "GameServerManager.h"
//#include "ManagPacket.h"

/*
*
*/

// DB Cache 건너야되나?

class ManagementServer : public BaseServer<ManagementServer> {
public:

private:
	MAKE_SERVER(ManagementServer)
		void HandlePacketLogin(BufferInfo* bufInfo);
	void HandlePacketGame(BufferInfo* bufInfo);

	//manager
	//ClntServerManager m_gameServerManager;
	GameServerManager m_gameServerManager;
	// LoginServerManager m_loginServerManager;
};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
