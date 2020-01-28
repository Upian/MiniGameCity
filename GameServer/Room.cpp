#include "RoomManager.h"
#include "Room.h"
#include "Player.h"

Room::Room(size_t maxPlayer, Player* master) : 
	m_maxPlayer(maxPlayer),
	m_roomMaster(master) {
	this->PlayerEnterRoom(master);
	m_roomManager = RoomManager::GetInstance();
	m_roomState = RoomState::roomStateLobby;

}

Room::~Room() {

}

void Room::StartGame(std::function<void(void)> game) {
	if (RoomState::roomStateLobby != m_roomState)
		return;
	if (nullptr != m_inGame)
		return;

	m_inGame = new std::thread(game);

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