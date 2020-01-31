#include "RoomManager.h"
#include "Room.h"
#include "Player.h"
#include "Log.h"
Room::Room(RoomManager* roomManager, int maxPlayer, Player* master, int roomNumber) :
	m_roomManager(roomManager),
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

bool Room::PlayerEnterRoom(Player* player) {
	if (nullptr == player) 
		return false;

	if (m_maxPlayer == m_playerManager.GetPlayerCount())
		return false;
	
	m_playerManager.InsertPlayer(player);
	return true;
}

void Room::PlayerLeaveRoom(Player* player) {
	if (nullptr == player)
		return;
	
	m_playerManager.RemovePlayer(player);//remove player
	
	if (m_roomMaster == player) //If the player is room master
		m_roomMaster = m_playerManager.GetPlayerList().front(); //

	return;
}

#pragma region operator

#pragma endregion
