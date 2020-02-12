#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "BaseServer.h"
#include "ServerCommon.h"
#include "PlayerManager.h"

class LoginServer : public BaseServer<LoginServer> {
public:
	void InitializeLoginServer();

private:
	MAKE_SERVER(LoginServer)
	void HandlePacketLogin(BufferInfo* bufInfo);

	void ConnectToManagementServer();

	SOCKET managementServer = 0;

	PlayerManager playerManager;
};

#endif