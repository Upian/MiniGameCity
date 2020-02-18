#pragma once
#ifndef __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
#define __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__

#include "BaseServer.h"
#include "ClntServerManager.h"
#include "ManagementPacket.h"

/*
*
*/

class ManagementServer : public BaseServer<ManagementServer> {
public:

	void InitializeManagmentServer();

private:
	MAKE_SERVER(ManagementServer)
		void HandlePacketLogin(BufferInfo* bufInfo);
	void HandlePacketToGameServer(BufferInfo* bufInfo);
	void ConnectToDBCache();

	Channel channel[CHANNEL_SIZE]{}; // test

	
	SOCKET dbCache = 0;
};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
