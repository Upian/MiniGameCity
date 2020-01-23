#pragma once
#ifndef __GAMESERVER_LOBBY_H__
#define __GAMESERVER_LOBBY_H__

#include <functional>
#include <list>
#include "GameServer.h"
#include "RoomManager.h"

class Player;
class RoomManager;

enum RoomState {
	roomStateNone = 0,

	roomStateLobby,
	roomStateGame,

	roomStateCount,
};


class Room {
public:
	Room(size_t maxPlayer, Player* master);
	~Room() {}

	void StartGame(std::function<void(void)> game);
	
	bool EnterRoom(Player* player);
	void LeaveRoom(Player* player);

	std::list<Player*>& GetPlayerList() { return m_playerList; }
	size_t GetPlayerCount() const { return m_playerList.size(); }


private:
	RoomManager* m_roomManager = nullptr;

	std::list<Player*> m_playerList;
	Player* m_roomMaster = nullptr;
	size_t m_maxPlayer = 0;


};
#endif // !__GAMESERVER_LOBBY_H__
