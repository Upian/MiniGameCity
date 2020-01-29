#pragma once
#ifndef __BASELIB_PLAYER_MANAGER_H__
#define __BASELIB_PLAYER_MANAGER_H__

#include <list>
#include <memory>

#include "ServerCommon.h"
#include "Singleton.h"
#include "Player.h"

class PlayerManager {
public:	
	void InsertPlayer(Player* player) { m_playerList.push_back(player); }
	void InsertPlayer(SOCKET socket) { m_playerList.push_back(new Player(socket)); }
	
	void RemovePlayer(Player* player);
	void RemovePlayer(GPID key);
	
	void PlayerDisconnect(SOCKET key);
	void SendToPlayers(BasePacket* packet);

	std::list<Player*>& GetPlayerList() { return m_playerList; }
	size_t GetPlayerCount() const { return m_playerList.size(); }
	bool IsPlayerEmpty() const { return m_playerList.empty(); }

	Player* FindPlayer(GPID playerId);
	Player* FindPlayerBySocket(SOCKET socket);

private:
	std::list<Player*> m_playerList;

};


#endif // !__BASELIB_PLAYER_MANAGER_H__

