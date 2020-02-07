#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "GameServer.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "RoomPacket.h"

GameServer::GameServer() {}
GameServer::~GameServer() {}

#pragma region Client connection
void GameServer::HandleAcceptClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	m_playerManager.InsertPlayer(clientSocket); //#temp
	printf("Connect client[%d] Total players[%d]\n", clientSocket, m_playerManager.GetPlayerList().size());
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
		
	default: break;
		Util::LoggingInfo("GameServer.log", "Recv wrong base packet ID: %d", type);
	}
	

}
#pragma region Handle packet
/*
*	Login server -> Game server
*	�÷��̾��� ������ �켱������ ����
*	������ �� ������ ���� �ʰ� ������ ��� ����
*/
void GameServer::HandlePacketPrepareTransfer() {

}

/*
*	Game client -> Game server
*	�� ���� ��Ŷ ó��
*/ 
void GameServer::HandleBasePacketRoom(BufferInfo* bufInfo) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (nullptr == player)
		return;
	m_roomManager.HandleRoomPacket(bufInfo->buffer, player);
}

void GameServer::HandleBasePacketSocial(BufferInfo * bufInfo) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (nullptr == player)
		return;

	m_socialManager.HandleSocialPacket(bufInfo->buffer, player);
}

#pragma endregion Handle packet functions

void GameServer::InitializeGameServer() {
	Sleep(1000);
	this->ConnectToManagementServer();
	this->ConnectToSocialServer();
	this->ConnectToRankingServer();

	m_roomManager.Initialize();
	m_socialManager.Initialize();
}

void GameServer::ConnectToManagementServer() {
	m_managementServerSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_managementServerSocket) {
		Util::LoggingError("GameServer.log", "ERROR - Can not create socket to connect management server");
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(Util::GetConfigToString("GameServer.ini", "Network", "ManagementServerIP", "10.255.252.100").c_str());
	address.sin_port = htons(Util::GetConfigToInt("GameServer.ini", "Network", "ManagementServerPort", 19999));

	if (SOCKET_ERROR == connect(m_managementServerSocket, (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("GameServer.log", "ERROR - Can not Connect to management server");
		_exit(0);
	}

	Util::LoggingInfo("GameServer.log", "Success - connect to management server");
	
	//send packet whitch this server is GameServer
	
	std::string testStr = "connect GameServer!!!!!!!!!!!!";
	send(m_managementServerSocket, testStr.c_str(), testStr.length(), 0);
}

void GameServer::ConnectToSocialServer() {
	m_socialServerSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_socialServerSocket) {
		Util::LoggingError("GameServer.log", "ERROR - Can not create socket to connect management server");
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(Util::GetConfigToString("GameServer.ini", "Network", "SocialServerIP", "10.255.252.100").c_str());

}

void GameServer::ConnectToRankingServer() {


}

