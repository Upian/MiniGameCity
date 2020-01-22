#pragma once
#ifndef __GAMESERVER_GAME_SERVER_H__
#define __GAMESERVER_GAME_SERVER_H__

#include "BaseServer.h"

//const char* serverName = "GameServer";
//const char* logFile = "GameServer.log";

class GameServer : public BaseServer<GameServer> {
public:
	void InitializeGameServer();

private:
	MAKE_SERVER(GameServer)
	void ConnectToManagementServer();
	void ConnectToRankingServer();
	void ConnectToSocialServer();
	
	ServerType m_serverType = ServerType::serverTypeGame;

	SOCKET m_managementServer = 0;
};


#endif // !__GAMESERVER_GAME_SERVER_H__
