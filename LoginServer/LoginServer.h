#ifndef LOGIN_SERVER_H
#define LOGIN_SERVER_H

#include "BaseServer.h"
#include "ServerCommon.h"

class LoginServer : public BaseServer<LoginServer> {
public:
	void InitializeLoginServer();

private:
	MAKE_SERVER(LoginServer)
		void HandlePacketLogin(BufferInfo* bufInfo);

	void ConnectToManagementServer();

	ServerType serverType = ServerType::serverTypeLogin;
	SOCKET managementServer = 0;
};

#endif