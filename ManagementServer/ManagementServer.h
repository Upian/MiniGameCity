#pragma once
#ifndef __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
#define __MANAGEMENTSERVER_MANAGEMENT_SERVER_H__

#include "BaseServer.h"
#include "GameServerManager.h"

/*
*
*/

class ManagementServer : public BaseServer<ManagementServer> {
public:

private:
	MAKE_SERVER(ManagementServer)
	void HandlePacketLogin(BufferInfo* bufInfo);
	void HandlePacketGame(BufferInfo* bufInfo);
	void HandleShowChannel(LoginManagementPacketTypeShowChannelRequest& bufInfo, std::shared_ptr<ClntServer> loginServer);
	void HandleChannelIn(LoginManagementPacketTypeChannelInRequest& bufInfo, std::shared_ptr<ClntServer> loginServer);

	//manager
	GameServerManager m_gameServerManager;
	ClntServerManager m_loginServerManager;
};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
