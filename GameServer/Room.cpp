#include "RoomManager.h"
#include "Room.h"
#include "Player.h"
#include "Log.h"
Room::Room(int roomNumber, std::string roomName, std::shared_ptr<Player> master, int maxPlayer) :
	m_roomName(roomName),
	m_maximumPlayer(maxPlayer),
	m_roomMaster(master),
	m_roomNumber(roomNumber),
	m_roomState(RoomState::roomStateWait){
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
	if (RoomState::roomStateWait != m_roomState)
		return;
	if (nullptr != m_inGameThread)
		return;


	m_inGameThread = new std::thread([this]()->void {
		m_roomState = RoomState::roomStateGaming;
		m_roomPlayerManager.SetAllPlayerState(PlayerState::playerStatePlayGame);
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

	if (m_maximumPlayer <= m_roomPlayerManager.GetPlayerCount()) {
		player->SetPlayerState(PlayerState::playerStateLobby);
		return false;
	}
	if (PlayerState::playerStateRoom == player->GetPlayerState()) //player already joined another room
		return false;
	
	player->SetPlayerState(PlayerState::playerStateRoom);
	m_roomPlayerManager.InsertPlayer(player);
	return true;
}

void Room::PlayerLeaveRoom(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	m_roomPlayerManager.RemovePlayer(player);//remove player
	player->SetPlayerState(PlayerState::playerStateLobby);

	if (true == m_roomPlayerManager.IsPlayerEmpty()) {
		m_roomState = RoomState::roomStateNone;
		return;
	}
		
	if (m_roomMaster == player) //If the player is room master
		m_roomMaster = m_roomPlayerManager.GetPlayerList().front(); //

	return;
}

void Room::SetPassword(__int16 password) {
	if (0 < password / 10000)
		return;

	m_password = password; 
	m_isUsePassword = true; 
}

bool Room::CheckPassword(__int16 password) {
	if(false == m_isUsePassword)
		return true;

	return password == m_password;
}
#pragma region operator

#pragma endregion
