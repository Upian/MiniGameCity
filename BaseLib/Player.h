#pragma once
#ifndef __BASELIB_PLAYER_H__
#define __BASELIB_PLAYER_H__

#include "ServerCommon.h"
#include "Client.h"

using GPID = unsigned __int64; //Unique ID for each player (Game Player ID)
class BasePacket;

class Player : public Client{
public:
	Player() {}
	Player(SOCKET socket) : Client(socket) {}
	~Player() {}

	SOCKET GetSocket() const { return m_socket; }
	GPID GetGPID() const { return m_gamePlayerId; }

private:
	SOCKET m_socket = 0;

	GPID m_gamePlayerId = 0; //
	bool m_isFirstLoginPlayer = true;
};
#endif // !__BASELIB_PLAYER_H__
