#include "RoomManager.h"
#include "Room.h"
#include "Player.h"
#include "Log.h"
Room::Room(std::string roomName, int maxPlayer, std::shared_ptr<Player> master, int roomNumber) :
	m_roomName(roomName),
	m_maxPlayer(maxPlayer),
	m_roomMaster(master),
	m_roomNumber(roomNumber),
	m_roomState(RoomState::roomStateLobby){
	this->PlayerEnterRoom(master);
}

Room::~Room() {
	if (nullptr != m_inGameThread) {
		if (true == m_inGameThread->joinable())
			m_inGameThread->join();
		delete m_inGameThread;
	}
		
	m_inGameThread = nullptr;
}

void Room::StartGame(std::function<void(void)> game) {
//	if (nullptr == game)
//		return;
	if (RoomState::roomStateLobby != m_roomState)
		return;
	if (nullptr != m_inGameThread)
		return;
	
	m_inGameThread = new std::thread([this]()->void {
		m_roomState = RoomState::roomStateGame;
		for (int i = 0; i < 10; ++i) {
			Sleep(500);
			Util::LoggingInfo("0_Test.log", "RoomNumber: %d  -  %d", this->GetRoomNumber(), i);
		}
		Util::LoggingInfo("0_Test.log", "RoomNumber: %d  -  Game END", this->GetRoomNumber());
		m_roomState = RoomState::roomStateNone;
		return;
	});
}

bool Room::PlayerEnterRoom(std::shared_ptr<Player> player) {
	if (nullptr == player) 
		return false;

	if (m_maxPlayer == m_roomPlayerManager.GetPlayerCount())
		return false;
	
	m_roomPlayerManager.InsertPlayer(player);
	return true;
}

void Room::PlayerLeaveRoom(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	m_roomPlayerManager.RemovePlayer(player);//remove player
	
	if (true == m_roomPlayerManager.IsPlayerEmpty()) {
		m_roomState = RoomState::roomStateNone;
		return;
	}
		
	if (m_roomMaster == player) //If the player is room master
		m_roomMaster = m_roomPlayerManager.GetPlayerList().front(); //

	return;
}

#pragma region operator

#pragma endregion
