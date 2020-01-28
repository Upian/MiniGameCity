#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "GameServer.h"
#include "RoomManager.h"
#include "PlayerManager.h"
#include "BasePacket.h"
GameServer::GameServer() {}
GameServer::~GameServer() {}

/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void GameServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf




	//switch(packet type) ...
	printf("Game server -- recv packet\n");
}

void GameServer::HandleAcceptClient(SOCKET clientSocket) {

}

void GameServer::InitializeGameServer() {
	this->ConnectToManagementServer();
	this->ConnectToRankingServer();
	this->ConnectToSocialServer();

	m_roomManager = RoomManager::CreateInstance();
}

void GameServer::ConnectToManagementServer() {
	m_managementServer = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_managementServer) {
		Util::LoggingError("GameServer.log", "ERROR - Can not create socket");
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(Util::GetConfigToString("GameServer.ini", "Network", "ManagementServerIP", "10.255.252.100").c_str());
	address.sin_port = htons(Util::GetConfigToInt("GameServer.ini", "Network", "ManagementServerPort", 19999));

	if (SOCKET_ERROR == connect(m_managementServer, (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("GameServer.log", "ERROR - Can not Connect to management server");
		_exit(0);
	}

	Util::LoggingInfo("GameServer.log", "Success - connect to management server");
	
	//send packet whitch this server is GameServer
	
	std::string testStr = "connect GameServer!!!!!!!!!!!!";
	send(m_managementServer, testStr.c_str(), testStr.length(), 0);
}

void GameServer::ConnectToRankingServer() {


}

void GameServer::ConnectToSocialServer() {


}
