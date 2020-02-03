#pragma once
#ifndef __GAMESERVER_GAME_SERVER_H__
#define __GAMESERVER_GAME_SERVER_H__

#include "BaseServer.h"
#include "RoomManager.h"
#include "PlayerManager.h"

//const char* serverName = "GameServer";
//const char* logFile = "GameServer.log";

struct BaseRoomPacket;

class GameServer : public BaseServer<GameServer> {
public:
	void InitializeGameServer();
	
	//Get manager
	RoomManager& GetRoomManager() { return m_roomManager; }
	PlayerManager& GetPlayerManager() { return m_playerManager; }

private:
	MAKE_SERVER(GameServer)
	void ConnectToManagementServer();
	void ConnectToRankingServer();
	void ConnectToSocialServer();

	void HandlePacketPrepareTransfer(); //PacketPrepareTransfer
	void HandlePacketRoom(BufferInfo* bufInfo);

	ServerType m_serverType = ServerType::serverTypeGame;

	SOCKET m_managementServer = 0;

	//manager
	RoomManager m_roomManager;
	PlayerManager m_playerManager;
};


#endif // !__GAMESERVER_GAME_SERVER_H__
