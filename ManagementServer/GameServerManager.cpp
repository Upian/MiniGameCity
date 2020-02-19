#include "GameServerManager.h"
#include "Log.h"

GameServerManager::GameServerManager() {

}

GameServerManager::~GameServerManager() {

}

void GameServerManager::Initialize() {

}

void GameServerManager::InsertServer(SOCKET socket) {
	if (true == this->isExistServer(socket))
		return;

	std::shared_ptr<GameServer> gameServer = std::make_shared<GameServer>(socket);
	gameServer->SetChannelName("Channel" + MakeChannelNumber());
	gameServer->SetCurrentPeople(0);
	gameServer->SetMaximumPeople(300);
	_gameServer.emplace_back(gameServer);
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

		  login server <-> management server

*/

void GameServerManager::HandleShowChannel(LoginManagementPacketTypeShowChannelRequest& packet, std::shared_ptr<ClntServer> loginServer) {
	if (loginServer == nullptr)
		return;

	LoginManagementPacketTypeShowChannelResponse packetLoginResponse;
	packetLoginResponse.channelSize = GetGameServerCount();
	for (auto c : _gameServer) {
		packetLoginResponse.channel.emplace_back(c->GetChannelName(), c->GetCurrentPeople(), c->GetMaximumPeople());
	}
	loginServer->SendPacket(packetLoginResponse);
}

void GameServerManager::HandleChannelIn(LoginManagementPacketTypeChannelInRequest& packet, std::shared_ptr<ClntServer> loginServer) {
	if (loginServer == nullptr)
		return;

	std::shared_ptr<GameServer> gameServer;
	for (auto g : _gameServer) {
		if (g->GetChannelName() == packet.channelName) {
			gameServer = g;
			break;
		}
	}

	GameManagementPacketTypeChannelInRequest packetGameRequest;
	packetGameRequest.gpid = packet.gpid;
	gameServer->SendPacket(packetGameRequest);


	LoginManagementPacketTypeChannelInResponse packetLoginResponse;
	packetLoginResponse.flag = true;
	packetLoginResponse.ip = Util::GetConfigToString("ManagementServer.ini", "Network", "GameServerIP", "127.0.0.1");
	packetLoginResponse.port = Util::GetConfigToInt("ManagementServer.ini", "Network", "GameServerPort", 10010);

	loginServer->SendPacket(packetLoginResponse);

}

/*

		  game server <-> management server

*/

void GameServerManager::HandlePacketGameUpdate(GameManagementPacketTypeCurrentPeopleRequest& packet, std::shared_ptr<ClntServer> gameServer) {
	if (nullptr == gameServer)
		return;

	for (auto g : _gameServer) {
		if (gameServer->GetSocket() == g->GetSocket())
			g->SetCurrentPeople(packet.currentPeople);
	}
}


