#include "GameServerManager.h"
#include "Log.h"

GameServerManager::GameServerManager() {

}

GameServerManager::~GameServerManager() {

}

void GameServerManager::Initialize() {

}

//void GameServerManager::InsertServer(SOCKET socket) {
//	if (true == this->isExistServer(socket))
//		return;
//
//	std::shared_ptr<GameServer> gameServer = std::make_shared<GameServer>(socket);
//	gameServer->SetChannelName("Channel" + MakeChannelNumber());
//	gameServer->SetCurrentPeople(0);
//	gameServer->SetMaximumPeople(300);
//	_gameServer.emplace_back(gameServer);
//}

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

void GameServerManager::HandlePacketGameRegister(GameToManagementRegisterServerInfo& packet, std::shared_ptr<GameServer> gameServer) {
	if (nullptr == gameServer)
		return;

	gameServer->SetChannelName("Channel" + MakeChannelNumber());
	gameServer->SetCurrentPeople(0);
	gameServer->SetMaximumPeople(300);

	for (auto g : _gameServer) {
		if (gameServer->GetSocket() == g->GetSocket()) {
			g->SetServerIpAddress(packet.m_ipAddress);
			g->SetServerPortNum(packet.m_portNum);
		}
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


