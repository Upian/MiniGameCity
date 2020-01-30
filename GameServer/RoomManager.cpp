#include "RoomManager.h"
#include "Log.h"
#include "RoomPacket.h"

RoomManager::~RoomManager() {
	if (nullptr != m_roomWatcher)
		delete m_roomWatcher;
	m_roomWatcher = nullptr;
}

void RoomManager::Initialize() {
	for (int i = m_maxRoomNumber; i > 0; --i) {
		m_roomNumberList.push(i);
	}
	m_roomWatcher = new std::thread([this]()->void { this->ClearDeactivatedRoom(); });
}

void RoomManager::ClearDeactivatedRoom() {
	while (true) {
		Sleep(500);
		if (true == m_roomList.empty())
			continue;

		m_roomList.remove_if([this](Room& room)-> bool{
			if (RoomState::roomStateNone == room.GetRoomState()) {
				Util::LoggingInfo("0_Test.log", "Remove!!! roomState: %d", room.GetRoomState());
				m_roomNumberList.push(room.GetRoomNumber());
				return true;
			}				
			return false;
		});
	}
}

void RoomManager::MakeRoom(size_t limitedNumberPlayer, Player* master) {
	if (m_minPlayer > limitedNumberPlayer && //Check player number
		m_maxPlayer < limitedNumberPlayer) 
		return;
	if (nullptr == master)
		return;

	m_roomList.emplace_back(this, limitedNumberPlayer, master, m_roomNumberList.top());
	m_roomNumberList.pop();
	//send packet to room master
}


Room& RoomManager::FindRoom(Player* pplayer) {
	for (Room& room : m_roomList) {
		if (room.GetRoomMaster() == pplayer) {
			return room;
		}
	}
}
