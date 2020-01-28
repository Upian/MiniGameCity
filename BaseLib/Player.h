#pragma once
#ifndef __BASELIB_PLAYER_H__
#define __BASELIB_PLAYER_H__

#include "ServerCommon.h"
#include "Client.h"

enum PlayerState {
	playerStateNone = 0,

	playerStateIdle = 0,
	playerStateLobby = 0,
	playerStateGame = 0,

	playerStateCount = 0,

};

using GPID = unsigned __int64; //Unique ID for each player (Game Player ID)
class BasePacket;
class Room;

class Player : public Client{
public:
	Player() {}
	Player(SOCKET socket) : Client(socket) {}
	~Player() {}

	SOCKET GetSocket() const { return m_socket; }
	GPID GetGPID() const { return m_gamePlayerId; }

private:
	GPID m_gamePlayerId = 0; //
	bool m_isFirstLoginPlayer = true;

	Room* m_room = nullptr;
};
#endif // !__BASELIB_PLAYER_H__
