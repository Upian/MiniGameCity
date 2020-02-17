#ifndef __LOGIN_SERVER_H__
#define __LOGIN_SERVER_H__

#include "BaseServer.h"
#include "LoginManagementHandler.h"
#include "LoginDBCacheHandler.h"

class LoginServer : public BaseServer<LoginServer> {
public:
	void InitializeLoginServer();

	PlayerManager& GetPlayerManager() {
		return m_playerManager;
	}
	SOCKET GetManagementServer() {
		return m_managementServer;
	}
	SOCKET GetDBCache() {
		return m_dbCache;
	}

private:
	MAKE_SERVER(LoginServer)
	void HandlePacketLogin(BufferInfo* bufInfo, LoginPacketType type);
	void HandlePacketChannel(BufferInfo* bufInfo, LoginPacketType type);

	SOCKET m_managementServer = 0;
	SOCKET m_dbCache = 0;

	PlayerManager m_playerManager;

	// handler
	LoginManagementHandler m_loginManagementHandler;
	LoginDBCacheHandler m_loginDBCacheHandler;
};

#endif // __LOGIN_SERVER_H__