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
	void InsertPlayer(std::shared_ptr<Player> player) { m_playerList.push_back(player); }
	void InsertPlayer(SOCKET socket);
	
	void RemovePlayer(std::shared_ptr<Player> player);
	void RemovePlayer(GPID key);
	
	void PlayerDisconnect(SOCKET key);
	void SendToAllPlayers(BasePacket& packet);
	void SendToLobbyPlayers(BasePacket& packet);
	void SendToGuildPlayers(BasePacket& packet);

	void SetAllPlayerState(PlayerState state);
	std::list<std::shared_ptr<Player> >& GetPlayerList() { return m_playerList; }
	size_t GetPlayerCount() const { return m_playerList.size(); }
	bool IsPlayerEmpty() const { return m_playerList.empty(); }
	bool IsExistPlayer(std::shared_ptr<Player> player);

	std::shared_ptr<Player> FindPlayer(GPID playerId);
	std::shared_ptr<Player> FindPlayerBySocket(SOCKET socket);

private:
	std::list<std::shared_ptr<Player> > m_playerList;

};


#endif // !__BASELIB_PLAYER_MANAGER_H__

