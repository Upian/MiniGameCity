#pragma once
#ifndef __BASELIB_PLAYER_H__
#define __BASELIB_PLAYER_H__


#include "ServerCommon.h"
#include "ClientBase.h"

enum PlayerState {
	playerStateNone = 0,

	playerStateIdle = 0,
	playerStateLobby = 0,
	playerStateGame = 0,

	playerStateCount = 0,

};

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)
class BasePacket;

class Player : public ClientBase{
public:
	Player(const Player&) {}
	Player(SOCKET socket) : ClientBase(socket) {}
	~Player() {}

	void Initialize();

	SOCKET GetSocket() const { return m_socket; }
	GPID GetGPID() const { return m_gamePlayerId; }

	virtual void HandlePacket(BufferInfo* packet) override;

	//operator
	bool operator==(const Player& player) const { return m_gamePlayerId == player.GetGPID(); }
private:
	GPID m_gamePlayerId = 0; //
	bool m_isFirstLoginPlayer = true;
};
#endif // !__BASELIB_PLAYER_H__
