#pragma once
#ifndef __GAMESERVER_GAME_SERVER_H__
#define __GAMESERVER_GAME_SERVER_H__

#include "BaseServer.h"
#include "RoomManager.h"
#include "PlayerManager.h"
#include "SocialServerHandler.h"
#include "ManagementServerHandler.h"

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

	void HandlePacketPrepareTransfer(); //PacketPrepareTransfer Management server -> game server (client informantion)
	void HandleBasePacketRoom(BufferInfo* bufInfo);
	void HandleBasePacketSocial(BufferInfo* bufInfo);
	void HandleBasePacketGame(BufferInfo* bufInfo);

	void PreLoadClientDataToPlayer(std::shared_ptr<Player> pplayer);

	SOCKET m_managementServerSocket = 0;
	SOCKET m_socialServerSocket = 0;
	//manager
	RoomManager m_roomManager;
	PlayerManager m_playerManager;
	//Handler
	ManagementServerHandler m_managementServerHandler;
	SocialServerHandler m_socialServerHandler;
};


#endif // !__GAMESERVER_GAME_SERVER_H__
