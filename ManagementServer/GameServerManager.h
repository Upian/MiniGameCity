// #pragma once
// #ifndef __MANAGEMENTSERVER_GAME_SERVER_MANAGER_H__
// #define __MANAGEMENTSERVER_GAME_SERVER_MANAGER_H__
// 
// #include "ClntServerManager.h"
// 
// class GameServerManager {
// public:
// 
// private:
// 	ClntServerManager m_gameServerManager;
// };
// 
// #endif // !__MANAGEMENTSERVER_GAME_SERVER_MANAGER_H__

#ifndef __GAME_SERVER_MANAGER_H__
#define __GAME_SERVER_MANAGER_H__
#include <list>
#include <vector>
#include <thread>

#include "GameServer.h"
#include "ClntServerManager.h"
#include "ManagementPacket.h"
#include "Config.h"

class GameServerManager : public ClntServerManager {
public:
	GameServerManager();
	~GameServerManager();

	void Initialize();

	void InsertServer(SOCKET socket);

	void HandleShowChannel(LoginManagementPacketTypeShowChannelRequest& packet, std::shared_ptr<ClntServer> loginServer);
	void HandleChannelIn(LoginManagementPacketTypeChannelInRequest& packet, std::shared_ptr<ClntServer> loginServer);
	void HandlePacketGameUpdate(GameManagementPacketTypeCurrentPeopleRequest& packet, std::shared_ptr<ClntServer> gameServer);

	std::shared_ptr<GameServer> FindGameServerBySocket(SOCKET socket);
	std::shared_ptr<GameServer> FindGameServerByChannelName(std::string name);

	size_t GetGamerServerCount() const {
		return _gameServer.size();
	}
	std::list<std::shared_ptr<GameServer> >& GetGameServerList() {
		return _gameServer;
	}
	size_t GetGameServerCount() const {
		return _gameServer.size();
	}
	std::string MakeChannelNumber();

private:

	std::list<std::shared_ptr<GameServer> > _gameServer;
};

#endif // !__GAMESERVER_MANAGER_H__
