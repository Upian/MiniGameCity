#pragma once
#ifndef __GAMESERVER_GAME_SERVER_H__
#define __GAMESERVER_GAME_SERVER_H__

#include "BaseServer.h"
#include "RoomManager.h"
#include "PlayerManager.h"
#include "SocialManager.h"

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
	void HandleBasePacketRoom(BufferInfo* bufInfo);
	void HandleBasePacketSocial(BufferInfo* bufInfo);

	ServerType m_serverType = ServerType::serverTypeGame;

	SOCKET m_managementServerSocket = 0;
	SOCKET m_socialServerSocket = 0;
	//manager
	RoomManager m_roomManager;
	PlayerManager m_playerManager;

	SocialManager m_socialManager;
};


#endif // !__GAMESERVER_GAME_SERVER_H__
