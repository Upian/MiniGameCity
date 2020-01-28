#include "RoomManager.h"
#include "Room.h"
RoomManager::RoomManager() {}
RoomManager::~RoomManager() {}

void RoomManager::MakeRoom(size_t limitedNumberPlayer, Player* master) {
	if (m_minPlayer > limitedNumberPlayer && m_maxPlayer < limitedNumberPlayer)
		return;
	if (nullptr == master)
		return;

	m_roomList.emplace_back(limitedNumberPlayer, master);
	//send packet to room master
}

void RoomManager::DestroyRoom() {

}

//Room* RoomManager::FindRoom() {

//}