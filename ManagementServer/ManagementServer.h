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
	void HandleShowChannel(LoginManagementPacketTypeShowChannelRequest& packet, std::shared_ptr<ClntServer> loginServer);
	void HandleChannelIn(LoginManagementPacketTypeChannelInRequest& packet, std::shared_ptr<ClntServer> loginServer);
	void HandleDisconnectUser(GameToManagementDisconnectUserRequest& packet);

	//manager
	GameServerManager m_gameServerManager;
	ClntServerManager m_loginServerManager;
};
#endif // !__MANAGEMENTSERVER_MANAGEMENT_SERVER_H__
