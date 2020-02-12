#include "GameServer.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "RoomPacket.h"
#include "SocialServerPacket.h"
#include "PreLoadPacket.h"

GameServer::GameServer() {}
GameServer::~GameServer() {}

#pragma region Client connection
namespace {
	int testGpid = 0;
	char testName = 'a';
}
void GameServer::HandleAcceptClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;

	/*
	*	After accepting client, check if it matches with information received from management server
	*/
// 
// 
// 
// 	
	auto player = m_playerManager.InsertPlayer(clientSocket); //#temp
	player->SetGPID(++testGpid); //#Test 
	std::string name;
	name += testName++;
	player->SetPlayerName(name); //#Test
	//Resister Social server
	this->RegisterPlayerAtSocialServer(player);
	
	//Send player data to each player
	this->PreLoadClientDataToPlayer(player);

	Util::LoggingDebug("GameServer.log", "Connect Client[%d], name[%s], GPID[%d]", clientSocket, player->GetPlayerName().c_str(), player->GetGPID());
}

void GameServer::HandleDisconnectClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	auto player = m_playerManager.FindPlayerBySocket(clientSocket);
	if (nullptr == player)
		return;

	auto room = player->GetRoom();//m_roomManager.FindRoomByPlayer(player);
	if(nullptr != room)
		room->PlayerLeaveRoom(player);

	m_playerManager.PlayerDisconnect(clientSocket);
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
	default: break;
		Util::LoggingInfo("GameServer.log", "Recv wrong base packet ID: %d", type);
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

	player->HandlePacket(bufInfo);
}

#pragma endregion Handle packet functions

void GameServer::InitializeGameServer() {
	Sleep(1000);
	m_managementServerHandler.ConnectToServer(
		Util::GetConfigToString("GameServer.ini", "Network", "ManagementServerIP", "10.255.252.100").c_str(),
		Util::GetConfigToInt("GameServer.ini", "Network", "ManagementServerPort", 19999)
	);
	m_socialServerHandler.Initialize();
	m_socialServerHandler.ConnectToServer(
		Util::GetConfigToString("GameServer.ini", "Network", "SocialServerIP", "10.255.252.100").c_str(),
		Util::GetConfigToInt("GameServer.ini", "Network", "SocialServerPort", 20100)
	);


	m_roomManager.Initialize();
}

void GameServer::RegisterPlayerAtSocialServer(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialPacketServerAcceptPlayerLogin packet;
	packet.m_gpid = pplayer->GetGPID();
	packet.m_name = pplayer->GetPlayerName(); //#Test

	m_socialServerHandler.SendPacketToServer(packet);
}

void GameServer::PreLoadClientDataToPlayer(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	PreLoadPacketLoadPlayerInfo packet;
	packet.m_playerName = pplayer->GetPlayerName();
	pplayer->SendPacket(packet);
}
