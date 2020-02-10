#pragma once
#ifndef __GAMESERVER_ROOM_H__
#define __GAMESERVER_ROOM_H__

#include <thread>
#include <functional>
#include <list>
#include <stack>

#include "PlayerManager.h"

class Player;
class RoomManager;
enum class ErrorTypeEnterRoom : char;

enum class RoomState {
	roomStateNone = 0,

	roomStateWait,
	roomStateGaming,

	roomStateCount,
};
enum class RoomGameType : char {
	GameTypeNone = 0,
	GameTypeTwentyQuestion,
	GameTypeRelayNovel,
	GameTypeBanKeyword,
	GameTypeCatchMind,

	GameTypeCount,
};

class InGame;

class Room : public std::enable_shared_from_this<Room>{
public:
	Room(int roomNumber, std::string roomName, std::shared_ptr<Player> master, int maxPlayer);
	~Room();

	void Initialize();

	void StartGame(std::function<void(void)> game);

	ErrorTypeEnterRoom PlayerEnterRoom(std::shared_ptr<Player> player, __int16 password = 0);
	void PlayerLeaveRoom(std::shared_ptr<Player> player);

	PlayerManager& GetPlayerManager() { return m_roomPlayerManager; }
	int GetPlayerCount() const { return m_roomPlayerManager.GetPlayerCount(); }
	int GetMaxPlayerCount() const { return m_maximumPlayer; }
	bool IsExistPlayer(std::shared_ptr<Player> player) { return m_roomPlayerManager.IsExistPlayer(player); }

	RoomState GetRoomState() const { return m_roomState; }
	bool IsRoomStateLobby() const { return RoomState::roomStateWait == m_roomState; }
	bool IsRoomStateGaming() const { return RoomState::roomStateGaming == m_roomState; }
	void UpdateRoomInfoToAllPlayers();
	void UpdateRoomInfoToPlayer(std::shared_ptr<Player> player);

	int GetRoomNumber() const { return m_roomNumber; }
	time_t GetCreatedTime() const { return m_createdTime; }

	void SetPassword(__int16 password);
	__int16 GetPassword() const { return m_password; }
	void SetIsUsePassword(bool isUse) { m_isUsePassword = isUse; }
	bool GetIsUsePassword() const { return m_isUsePassword; }
	bool CheckPassword(__int16 password);
	bool CheckIsRoomIsFull() { return m_maximumPlayer <= m_roomPlayerManager.GetPlayerCount(); }

	std::string& GetRoomName() { return m_roomName; }

	std::shared_ptr<Player> GetRoomMaster() const { return m_roomMaster; }
	//operator
	bool operator==(const Room& other) const { return m_roomNumber == other.GetRoomNumber(); }
private:
	PlayerManager						m_roomPlayerManager;
	std::shared_ptr<Player>				m_roomMaster = nullptr;
	std::stack<__int16>					m_playerPosionStack;

	RoomGameType						m_gameType = RoomGameType::GameTypeNone;
	RoomState							m_roomState = RoomState::roomStateNone;
	std::thread*						m_inGameThread = nullptr;
	
	std::string							m_roomName;
	int									m_maximumPlayer = 0;
	int									m_roomNumber = 0;
	time_t								m_createdTime = 0;
	bool								m_isUsePassword = false;
	__int16								m_password;

};
#endif // !__GAMESERVER_LOBBY_H__
