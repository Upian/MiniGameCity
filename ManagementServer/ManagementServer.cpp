#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ManagementServer.h"

ManagementServer::ManagementServer() {}
ManagementServer::~ManagementServer() {}

// Game Server와 통신 필요함.
// ShowChannel 누르면 게임서버로부터 인원들 파악해서 가져와야 함.

void ManagementServer::HandleAcceptClient(SOCKET gameServerSocket) {
	// login서버면 insert 할 필요 없다.


	m_gameServerManager.InsertServer(gameServerSocket);
	Util::LoggingDebug("ManagementServer.log", "Connect Client[%d]", gameServerSocket);

}
void ManagementServer::HandleDisconnectClient(SOCKET gameServerSocket) {
	m_gameServerManager.RemoveServer(gameServerSocket);
	printf("Disconnect client[%d] Total players[%d]\n", gameServerSocket, m_gameServerManager.GetGamerServerCount());
}

/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/

void ManagementServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::loginPacketTypeManagement: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	case BasePacketType::basePacketTypeGameToManagementServer: {
		this->HandlePacketGame(bufInfo);
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
	printf("Management server -- recv packet: %s\n", bufInfo->dataBuf.buf);
}

void ManagementServer::HandlePacketLogin(BufferInfo* bufInfo) {
	ManagementPacketType type = (ManagementPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case loginManagementPacketTypeShowChannelRequest: {
		LoginManagementPacketTypeShowChannelRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		std::shared_ptr<ClntServer> clntServer;
		clntServer->SetServerSocket(bufInfo->socket);
		m_gameServerManager.HandleShowChannel(packetLoginRequest, clntServer);
		break;
	}

	case loginManagementPacketTypeChannelInRequest: {
		LoginManagementPacketTypeChannelInRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		std::shared_ptr<ClntServer> clntServer;
		clntServer->SetServerSocket(bufInfo->socket);
		m_gameServerManager.HandleChannelIn(packetLoginRequest, clntServer);
		break;
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong login packet ID : %d", type);
	}
	}
}

void ManagementServer::HandlePacketGame(BufferInfo* bufInfo) {
	ManagementPacketType type = (ManagementPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case gameManagementPacketTypeCurrentPeopleRequest: {
		GameManagementPacketTypeCurrentPeopleRequest packetGameRequest;
		packetGameRequest.Deserialize(bufInfo->buffer);
		auto gameServer = m_gameServerManager.FindServerBySocket(bufInfo->socket);
		m_gameServerManager.HandlePacketGameUpdate(packetGameRequest, gameServer);
		break;
	}

	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong login packet ID : %d", type);
	}
	}
}