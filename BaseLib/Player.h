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

	//Using InGame Func
	void SetNickName(std::string afterNickName) { playerNickName = afterNickName; }
	std::string GetNickName() { return playerNickName; }

	void SetGameScore(int changeScore) { InGameScore += changeScore; }
	int GetGameScore() { return InGameScore; }

	void SetExitReservation(bool changeExit) { ExitReservation = changeExit; }
	bool GetExitReservation() { return ExitReservation; }

	//operator
	bool operator==(const Player& player) const { return m_gamePlayerId == player.GetGPID(); }
private:
	GPID m_gamePlayerId = 0; //
	bool m_isFirstLoginPlayer = true;

	//Using InGame Var
	std::string		playerNickName = NULL;		//플레이어 닉네임
	int				InGameScore = 0;			//인게임 점수
	bool			ExitReservation = false;	//게임 나가기 예약
};
#endif // !__BASELIB_PLAYER_H__
