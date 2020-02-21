#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ManagementServer.h"

ManagementServer::ManagementServer() {}
ManagementServer::~ManagementServer() {}

void ManagementServer::HandleAcceptClient(SOCKET socket) {
	// login서버면 insert 할 필요 없다.
	m_loginServerManager.InsertServer(socket);
	// m_gameServerManager.InsertGameSever(socket);
	Util::LoggingDebug("ManagementServer.log", "Connect Client[%d]", socket);

}
void ManagementServer::HandleDisconnectClient(SOCKET gameServerSocket) {
	m_gameServerManager.RemoveServer(gameServerSocket);
	printf("Disconnect client[%d] Total players[%d]\n", gameServerSocket, m_gameServerManager.GetGameServerCount());
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
		break;
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
		this->HandleShowChannel(packetLoginRequest, m_loginServerManager.FindServerBySocket(bufInfo->socket));
		break;
	}

	case loginManagementPacketTypeChannelInRequest: {
		LoginManagementPacketTypeChannelInRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		this->HandleChannelIn(packetLoginRequest, m_loginServerManager.FindServerBySocket(bufInfo->socket));
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
	case registerServerInfo: {
		GameToManagementRegisterServerInfo packetGameRequest;
		packetGameRequest.Deserialize(bufInfo->buffer);
		m_gameServerManager.HandlePacketGameRegister(packetGameRequest, bufInfo->socket);
		break;
	}
	case updateServerInfo: {
		GameToManagementUpdateServerInfoRequest packetGameRequest;
		packetGameRequest.Deserialize(bufInfo->buffer);
		auto gameServer = m_gameServerManager.FindGameServerBySocket(bufInfo->socket);
		m_gameServerManager.HandlePacketGameUpdate(packetGameRequest, gameServer);
		break;
	}
	case disconnectUserRequest: {
		GameToManagementDisconnectUserRequest packetGameRequest;
		packetGameRequest.Deserialize(bufInfo->buffer);
		this->HandleDisconnectUser(packetGameRequest);
		break;
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong login packet ID : %d", type);
	}
	}
}

void ManagementServer::HandleShowChannel(LoginManagementPacketTypeShowChannelRequest& packet, std::shared_ptr<ClntServer> loginServer) {
	if (loginServer == nullptr)
			return;
	
	LoginManagementPacketTypeShowChannelResponse packetLoginResponse;
	packetLoginResponse.channelSize = m_gameServerManager.GetGameServerCount();
	for (auto c : m_gameServerManager.GetGameServerList()) {
		if (c->GetChannelName() == "")
			continue;
		packetLoginResponse.channel.emplace_back(c->GetChannelName(), c->GetCurrentPeople(), c->GetMaximumPeople());
	}
	packetLoginResponse.gpid = packet.gpid;
	loginServer->SendPacket(packetLoginResponse);
}

void ManagementServer::HandleChannelIn(LoginManagementPacketTypeChannelInRequest& packet, std::shared_ptr<ClntServer> loginServer) {
	if (loginServer == nullptr) 
		return;

	std::shared_ptr<GameServer> gameServer;
	for (auto g : m_gameServerManager.GetGameServerList()) {
		if (g->GetChannelName() == packet.channelName) {
			gameServer = g;
			break;
		}
	}

	GameToManagementPreLoadRequest packetGameRequest;
	packetGameRequest.m_gpid = packet.gpid;
	packetGameRequest.m_token = packet.token;
	gameServer->SendPacket(packetGameRequest);

	LoginManagementPacketTypeChannelInResponse packetLoginResponse;
	packetLoginResponse.flag = true;
	packetLoginResponse.ip = gameServer->GetServerIpAddress();
	packetLoginResponse.port = gameServer->GetServerPortNum();
	packetLoginResponse.gpid = packet.gpid;
	loginServer->SendPacket(packetLoginResponse);
}

void ManagementServer::HandleDisconnectUser(GameToManagementDisconnectUserRequest& packet) {
	for (auto g : m_gameServerManager.GetGameServerList()) {
		g->SendPacket(packet);
	}
}
