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

	void HandlePacketGameRegister(GameToManagementRegisterServerInfo& packet, SOCKET socket);
	void HandlePacketGameUpdate(GameToManagementUpdateServerInfoRequest& packet, std::shared_ptr<GameServer> gameServer);

	std::shared_ptr<GameServer> FindGameServerBySocket(SOCKET socket);
	std::shared_ptr<GameServer> FindGameServerByChannelName(std::string name);

	std::list<std::shared_ptr<GameServer> >& GetGameServerList() {
		return _gameServer;
	}
	size_t GetGameServerCount() const {
		return _gameServer.size();
	};
	std::string MakeChannelNumber();

private:

	std::list<std::shared_ptr<GameServer> > _gameServer;
};

#endif // !__GAMESERVER_MANAGER_H__
