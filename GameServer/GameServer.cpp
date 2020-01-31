#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "GameServer.h"
#include "PlayerManager.h"
#include "RoomManager.h"
#include "RoomPacket.h"

GameServer::GameServer() {}
GameServer::~GameServer() {}

/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void GameServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->dataBuf.buf);

	switch (type) {
	case basePacketTypeRoom: {
		this->HandlePacketRoom(bufInfo);
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
void GameServer::HandlePacketRoom(BufferInfo* bufInfo) {
	PacketTypeRoom type = (PacketTypeRoom)PacketTypeDeserial(bufInfo->dataBuf.buf);

	switch (type) {
	case PacketTypeRoom::packetTypeRoomMakeRoomRequest: {
		RoomPacketMakeRoomRequest packet;
		packet.Deserialize(bufInfo->dataBuf.buf);
		//Player* player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
		Player* player = new Player(bufInfo->socket); //#Temp
		m_roomManager.MakeRoom(packet.m_maxPlayer, player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomRoomListRequest: {

	}
	case PacketTypeRoom::packetTypeRoomEnterRoomRequest: {

	}
	default:
		Util::LoggingInfo("GameServer.log", "Recv wrong room packet ID: %d", type);
	}
}

#pragma endregion Handle packet functions


void GameServer::HandleAcceptClient(SOCKET clientSocket) {

}


void GameServer::InitializeGameServer() {
	this->ConnectToManagementServer();
	this->ConnectToRankingServer();
	this->ConnectToSocialServer();

	m_roomManager.Initialize();
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
