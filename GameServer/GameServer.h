#pragma once
#ifndef __GAMESERVER_GAME_SERVER_H__
#define __GAMESERVER_GAME_SERVER_H__

#include "BaseServer.h"

//const char* serverName = "GameServer";
//const char* logFile = "GameServer.log";

class RoomManager;
class PlayerManager;

class GameServer : public BaseServer<GameServer> {
public:
	void InitializeGameServer();
	
	//Get manager
	RoomManager* GetRoomManager() const { return m_roomManager; }
	PlayerManager* GetPlayerManager() const { return m_playerManager; }

private:
	MAKE_SERVER(GameServer)
	void ConnectToManagementServer();
	void ConnectToRankingServer();
	void ConnectToSocialServer();
	
	ServerType m_serverType = ServerType::serverTypeGame;

	SOCKET m_managementServer = 0;

	//manager
	RoomManager* m_roomManager = nullptr;
	PlayerManager* m_playerManager = nullptr;
};


#endif // !__GAMESERVER_GAME_SERVER_H__
