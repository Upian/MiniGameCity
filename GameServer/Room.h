#pragma once
#ifndef __GAMESERVER_ROOM_H__
#define __GAMESERVER_ROOM_H__

#include <thread>
#include <functional>
#include <list>

#include "PlayerManager.h"

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
	Room(RoomManager* roomManager, int maxPlayer, Player* master, int roomNumber);
	~Room();

	void StartGame(std::function<void(void)> game);

	bool PlayerEnterRoom(Player* player);
	void PlayerLeaveRoom(Player* player);

	PlayerManager& GetPlayerManager() { return m_playerManager; }
	int GetPlayerCount() const { return m_playerManager.GetPlayerCount(); }

	RoomState GetRoomState() const { return m_roomState; }
	int GetRoomNumber() const { return m_roomNumber; }

	Player* GetRoomMaster() const { return m_roomMaster; }
	//operator
	bool operator==(const Room& other) const { return m_roomNumber == other.GetRoomNumber(); }
private:
	RoomManager* m_roomManager = nullptr;

	PlayerManager m_playerManager;
	Player* m_roomMaster = nullptr;
	int m_maxPlayer = 0;
	RoomState m_roomState = RoomState::roomStateNone;
	int m_roomNumber = 0;

	std::thread* m_inGameThread = nullptr;

	bool m_usePassword = false;
	char m_password[4];

};
#endif // !__GAMESERVER_LOBBY_H__
