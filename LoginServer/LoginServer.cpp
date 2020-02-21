#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "LoginServer.h"

LoginServer::LoginServer() {}
LoginServer::~LoginServer() {}

void LoginServer::HandleAcceptClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	auto player = m_playerManager.InsertPlayer(clientSocket);
	player->SetGPID(0);
	std::string name;
	player->SetPlayerName(name);
	Util::LoggingDebug("LoginServer.log", "Connect Client[%d], name[%s], GPID[%d]", clientSocket, player->GetPlayerName().c_str(), player->GetGPID());
}
void LoginServer::HandleDisconnectClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	auto player = m_playerManager.FindPlayerBySocket(clientSocket);
	if (player == nullptr)
		return;

	m_playerManager.PlayerDisconnect(clientSocket);
	printf("Disconnect client[%d] Total players[%d]\n", clientSocket, m_playerManager.GetPlayerList().size());
}

void LoginServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::basePacketTypeLogin: {
		LoginPacketType loginType = (LoginPacketType)PacketTypeDeserial(bufInfo->buffer);
		switch (loginType) {
		case clientLoginPacketTypeChannelInRequest:
		case clientLoginPacketTypeShowChannelRequest: {
			this->HandlePacketChannel(bufInfo, loginType);
			break;
		}
		case clientLoginPacketTypeLoginRequest:
		case clientLoginPacketTypeLogoutRequest:
		case clientLoginPacketTypeSignupRequest:
		case clientLoginPacketTypeDeleteRequest: {
			this->HandlePacketLogin(bufInfo, loginType);
			break;
		}
		default: {
			break;
		}
		}
		break;
	}
	default: {
		Util::LoggingInfo("LoginServer.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
}

void LoginServer::HandlePacketLogin(BufferInfo* bufInfo, LoginPacketType type) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (player == nullptr)
		return;
	m_loginDBCacheHandler.HandleLoginPacket(bufInfo->buffer, player, type);
}

void LoginServer::HandlePacketChannel(BufferInfo* bufInfo, LoginPacketType type) {
	auto player = m_playerManager.FindPlayerBySocket(bufInfo->socket);
	if (player == nullptr)
		return;
	m_loginManagementHandler.HandleLoginPacket(bufInfo->buffer, player, type);
}

void LoginServer::InitializeLoginServer() {
	Sleep(1000);
	m_loginManagementHandler.Initialize();
	m_loginManagementHandler.ConnectToServer(
		Util::GetConfigToString("LoginServer.ini", "Network", "ManagementServerIP", "127.0.0.1").c_str(),
		Util::GetConfigToInt("LoginServer.ini", "Network", "ManagementServerPort", 19999)
	);
	m_loginDBCacheHandler.Initialize();
	m_loginDBCacheHandler.ConnectToServer(
		Util::GetConfigToString("LoginServer.ini", "Network", "DBCacheIP", "127.0.0.1").c_str(),
		Util::GetConfigToInt("LoginServer.ini", "Network", "DBCachePort", 30001)
	);
}
