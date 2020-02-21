#include "GameServerManager.h"
#include "Log.h"

GameServerManager::GameServerManager() {

}

GameServerManager::~GameServerManager() {

}

void GameServerManager::Initialize() {

}

std::shared_ptr<GameServer> GameServerManager::FindGameServerBySocket(SOCKET socket) {
	for (auto g : _gameServer) {
		if (socket == g->GetSocket())
			return g;
	}
	return nullptr;
}

std::shared_ptr<GameServer> GameServerManager::FindGameServerByChannelName(std::string name) {
	for (auto g : _gameServer) {
		if (name == g->GetChannelName()) {
			return g;
		}
	}
	return nullptr;
}

std::shared_ptr<GameServer> GameServerManager::FindGameServerByAddress(std::string ip, int port) {
	for (auto server : _gameServer) {
		if (port == server->GetServerPortNum() && ip == server->GetServerIpAddress())
			return server;
	}
	return nullptr;
}

std::string GameServerManager::MakeChannelNumber() {
	std::vector<bool> tmp(GetGameServerCount() + 1, false);
	for (auto g : _gameServer) {
		tmp[stoi(g->GetChannelName().substr(7)) - 1] = true;
	}
	for (int i = 0; i < tmp.size(); ++i) {
		if (tmp[i] == false)
			return std::to_string(i + 1);
	}
	return "";
}

/*

		  game server <-> management server

*/

void GameServerManager::HandlePacketGameRegister(GameToManagementRegisterServerInfo& packet, SOCKET socket) {
	if (packet.m_portNum >= Util::GetConfigToInt("ManagementServer.ini", "Network", "GameServerPort", 10010)) {
		std::shared_ptr<GameServer> gameServer(new GameServer(socket));
		gameServer->SetServerIpAddress(packet.m_ipAddress);
		gameServer->SetServerPortNum(packet.m_portNum);
		gameServer->SetChannelName("Channel" + MakeChannelNumber());
		gameServer->SetCurrentPeople(0);
		gameServer->SetMaximumPeople(300);
		_gameServer.emplace_back(gameServer);
	}
}

void GameServerManager::HandlePacketGameUpdate(GameToManagementUpdateServerInfoRequest& packet, std::shared_ptr<GameServer> gameServer) {
	if (nullptr == gameServer)
		return;

	for (auto g : _gameServer) {
		if (gameServer->GetSocket() == g->GetSocket())
			g->SetCurrentPeople(packet.m_currentPlayer);
	}
}

void GameServerManager::HandlePacketGameTransferChannel(GameToManagementTransferChannelRequest& packet, std::shared_ptr<GameServer> srcServer) {
	if (nullptr == srcServer)
		return;

	GameToManagementTransferChannelResponse responseSrcPacket;
	responseSrcPacket.m_gpid = packet.m_gpid;
	responseSrcPacket.m_isSuccess = true;

	auto destServer = this->FindGameServerByAddress(packet.m_ipAddress, packet.m_portNum);
	if (nullptr == destServer) {
		responseSrcPacket.m_isSuccess = false;
		srcServer->SendPacket(responseSrcPacket);
		return;
	}
	Util::LoggingDebug("ManagementServer.log", "%d transfer %s[%s:%d] to %s[%s:%d]", 
		packet.m_gpid, 
		srcServer->GetChannelName(), srcServer->GetServerIpAddress(), srcServer->GetServerPortNum(),
		destServer->GetChannelName(), destServer->GetServerIpAddress(), destServer->GetServerPortNum());
	responseSrcPacket.m_channelName = destServer->GetChannelName();
	

	GameToManagementPreLoadRequest responseDestPacket;
	responseDestPacket.m_gpid = packet.m_gpid;
	responseDestPacket.m_token = packet.m_session;
	
	destServer->SendPacket(responseDestPacket);
	srcServer->SendPacket(responseSrcPacket);
}
