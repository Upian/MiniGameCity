#include "Room.h"
#include "Player.h"

Room::Room(size_t maxPlayer, Player* master) : 
	m_maxPlayer(maxPlayer),
	m_roomMaster(master) {
	this->EnterRoom(master);
	m_roomManager = RoomManager::GetInstance();
}

void Room::StartGame(std::function<void(void)> game) {


}

bool Room::EnterRoom(Player* player) {
	if (nullptr == player) 
		return false;

	if (m_maxPlayer == m_playerList.size())
		return false;
	
	m_playerList.push_back(player);
	return true;
}

void Room::LeaveRoom(Player* player) {
	if (nullptr == player)
		return;
	
	m_playerList.remove(player); //remove player
	
	
	if (m_roomMaster != player)
		return;

	m_roomMaster = player;
}