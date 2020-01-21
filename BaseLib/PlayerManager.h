#pragma once
#ifndef __BASELIB_PLAYER_MANAGER_H__
#define __BASELIB_PLAYER_MANAGER_H__

#include <list>
#include <memory>

#include "ServerCommon.h"
#include "Singleton.h"
#include "Player.h"

class PlayerManager : public Util::Singleton<PlayerManager>{
public:
	void PlayerLogin(SOCKET socket);
	void PlayerLogout(GPID key);
	void PlayerDisconnect(SOCKET key);

	void SendToPlayers(BasePacket* packet);

	void PlayerTransferServer();
	
	Player* FindPlayer(GPID playerId);
	Player* FindPlayerBySocket(SOCKET socket);

private:
	DECLARE_SINGLETON(PlayerManager)

	std::list<Player*> m_playerList;

};


#endif // !__BASELIB_PLAYER_MANAGER_H__

