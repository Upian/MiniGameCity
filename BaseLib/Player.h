#pragma once
#ifndef __BASELIB_PLAYER_H__
#define __BASELIB_PLAYER_H__


#include "ServerCommon.h"
#include "ClientBase.h"

enum class PlayerState {
	playerStateNone = 0,

	playerStateIdle,
	playerStateLobby,
	playerStateRoom,
	playerStatePlayGame,

	playerStateCount,

};

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)
class BasePacket;
class Room;

class Player : public ClientBase{
public:
	Player(const Player&) {}
	Player(SOCKET socket) : ClientBase(socket) {}
	~Player() {}

	void Initialize();

	SOCKET GetSocket() const { return m_socket; }
	GPID GetGPID() const { return m_gamePlayerId; }

	void SetPlayerState(PlayerState state) { m_playerState = state; }
	PlayerState GetPlayerState() { return m_playerState; }

	void SetPlayerName(std::string& name) { m_playerName = name; }
	std::string GetPlayerName() const { return m_playerName; }

	virtual void HandlePacket(BufferInfo* packet) override;


#pragma region Game
	void PlayerInfoClear();

	void SetPositionIndex(__int16 position) { m_potionIndex = position; }
	__int16 GetPositionIndex() const { return m_potionIndex; }

	void SetIsRoomMaster(bool master) { m_isRoomMaster = master; }
	bool GetIsRoomMaster() const { return m_isRoomMaster; }

	void toggleReady() { m_isReady = !m_isReady; }
	void SetIsReady(bool ready) { m_isReady = ready; }
	bool GetIsReady() const { return m_isReady; }

	void SetRoom(std::shared_ptr<Room> room) { m_room = room; }
	std::shared_ptr<Room> GetRoom() const { return m_room; }

	void UpdateGameScore(int changeScore) { InGameScore += changeScore; }
	int GetGameScore() { return InGameScore; }

	void SetExitReservation(bool changeExit) { ExitReservation = changeExit; }
	bool GetExitReservation() { return ExitReservation; }
#pragma endregion for using game room


	//operator
	bool operator==(const Player& player) const { return m_gamePlayerId == player.GetGPID(); }
private:
	GPID m_gamePlayerId = 0; //
	bool m_isFirstLoginPlayer = true;
	PlayerState m_playerState = PlayerState::playerStateNone;
	std::string m_playerName;

#pragma region Game
	std::shared_ptr<Room> m_room = nullptr;
	__int16 m_potionIndex = 0;
	bool m_isRoomMaster = false;
	__int32 m_imageIndex = 0;
	bool m_isReady = false;

	int				InGameScore = 0;			//인게임 점수
	bool			ExitReservation = false;	//게임 나가기 예약

#pragma endregion for using game room
};
#endif // !__BASELIB_PLAYER_H__
