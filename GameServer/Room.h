#pragma once
#ifndef __GAMESERVER_LOBBY_H__
#define __GAMESERVER_LOBBY_H__

#include <thread>
#include <functional>
#include <list>
#include "GameServer.h"

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
	~Room();

	void StartGame(std::function<void(void)> game);
	
	bool PlayerEnterRoom(Player* player);
	void PlayerLeaveRoom(Player* player);

	PlayerManager& GetPlayerManager() { return m_playerManager; }
	size_t GetPlayerCount() const { return m_playerManager.GetPlayerCount(); }


private:
	RoomManager* m_roomManager = nullptr;

	PlayerManager m_playerManager;
	Player* m_roomMaster = nullptr;
	size_t m_maxPlayer = 0;
	RoomState m_roomState = RoomState::roomStateNone;

	std::thread* m_inGame = nullptr;

	bool m_usePassword = false;
	char m_password[4];

};
#endif // !__GAMESERVER_LOBBY_H__
