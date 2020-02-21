#include "GameServer.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "RoomPacket.h"
#include "SocialServerPacket.h"
#include "ConnectionPacket.h"

GameServer::GameServer() {}
GameServer::~GameServer() {}

#pragma region Client connection

void GameServer::HandleAcceptClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;

	/*
	*	After accepting client, check if it matches with information received from management server
	*/
}

void GameServer::HandleDisconnectClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	auto player = m_playerManager.FindPlayerBySocket(clientSocket);
	if (nullptr == player)
		return;

	player->SetPlayerState(PlayerState::playerStateNone);

	m_socialServerHandler.UpdatePlayerInfoAtLogout(player);

	auto room = player->GetRoom();//m_roomManager.FindRoomByPlayer(player);
	if(nullptr != room)
		room->PlayerLeaveRoom(player);

	m_playerManager.PlayerDisconnect(player);
	printf("Disconnect client[%d] Total players[%d]\n", clientSocket, m_playerManager.GetPlayerList().size());


}
#pragma endregion
/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void GameServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);


	switch (type) {
	case BasePacketType::basePacketTypeConnection: {

		break;
	}
	case BasePacketType::basePacketTypeRoom: {
		this->HandleBasePacketRoom(bufInfo);
		break;
	}
	case BasePacketType::basePacketTypeSocial: {
		this->HandleBasePacketSocial(bufInfo);
		break;
	}
	case BasePacketType::basePacketTypeGame: {
		this->HandleBasePacketGame(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("GameServer.log", "Recv wrong base packet ID: %d", type);
		break;
	}
		
	}
	

}
#pragma region Handle packet
/*
*	Login server -> Game server
*	플레이어의 정보를 우선적으로 전달
*	유저가 이 과정을 하지 않고 접근할 경우 차단
*/
void GameServer::HandlePacketPrepareTransfer() {

}

void GameServer::HandleBasePacketConnection(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;
	PacketTypeConnection type = (PacketTypeConnection)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case PacketTypeConnection::connectionRequest: {//When client first connect to the game server
		ConnectionPacketConnectServerRequest packet;
		packet.Deserialize(bufInfo->buffer);
		this->AcceptClient(packet.m_sessionId, bufInfo->socket);
		break;
	}
	case PacketTypeConnection::InviteTransferRequest: {

		break;
	}
	default: {
		Util::LoggingInfo("GameServer.log", "Recv wrong connection packet ID: %d", type);
		break;
	}
	}

}

/*
*	Game client -> Game server
*	방 관련 패킷 처리
*/ 
void GameServer::HandleBasePacketRoom(BufferInfo* bufInfo) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (nullptr == player)
		return;
	m_roomManager.HandleRoomPacket(bufInfo->buffer, player);
}

void GameServer::HandleBasePacketSocial(BufferInfo* bufInfo) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (nullptr == player)
		return;

	m_socialServerHandler.HandleSocialPacket(bufInfo->buffer, player);
}

void GameServer::HandleBasePacketGame(BufferInfo* bufInfo) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (nullptr == player)
		return;

	player->GetRoom()->GetGameLibrary().SaveGamePacketToQueue(bufInfo->buffer, player);
}

#pragma endregion Handle packet functions

void GameServer::InitializeGameServer() {
	Sleep(1000);
	m_managementServerHandler.Initialize();
	m_socialServerHandler.Initialize();

	m_roomManager.Initialize();
}

void GameServer::AcceptClient(SessionID session, SOCKET sock) {
	auto pPlayer = m_playerManager.FindPreLoadClient(session);
	if (nullptr == pPlayer) {
		closesocket(sock);
		return;
	}
	
	pPlayer->SetSock(sock); //#Test
	//Resister Social server
	m_socialServerHandler.UpdatePlayerInfoAtLogin(pPlayer);
	//Send player data to each player
	this->PreLoadClientDataToPlayer(pPlayer);

	m_playerManager.InsertPlayer(pPlayer);
	Util::LoggingDebug("GameServer.log", "Connect Client[%d], name[%s], GPID[%d]", sock, pPlayer->GetPlayerName().c_str(), pPlayer->GetGPID());
}

void GameServer::PreLoadClientDataToPlayer(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	ConnectionPacketLoadPlayerInfo packet;
	packet.m_playerName = pplayer->GetPlayerName();
	pplayer->SendPacket(packet);
}

