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

		m_roomList.remove_if([this](std::shared_ptr<Room> room)-> bool{
			if (RoomState::roomStateNone == room->GetRoomState()) {
				Util::LoggingInfo("0_Test.log", "Remove!!! roomState: %d", room->GetRoomState());
				m_roomNumberList.push(room->GetRoomNumber());
				return true;
			}				
			return false;
		});
	}
}

void RoomManager::MakeRoom(size_t limitedNumberPlayer, Player* master) {
	if (nullptr == master)
		return;

	std::shared_ptr<Room> tempRoom;
	
	RoomPacketMakeRoomResponse packet;
	//Check player number

	if ((limitedNumberPlayer < m_minPlayer) ||
		(limitedNumberPlayer > m_maxPlayer)) {
		packet.m_success = false;

		Util::LoggingInfo("", "Fail Make room");
	}	
	else {
		int roomNumber = m_roomNumberList.top();
		m_roomNumberList.pop();

		tempRoom = std::make_shared<Room>(this, limitedNumberPlayer, master, roomNumber);
		m_roomList.push_back(tempRoom);

		packet.m_success = true;
		packet.m_roomNumber = roomNumber;

		Util::LoggingInfo("", "Success Make room - number[%d]", roomNumber);
	}
	
	//send packet to room master
	//If send fails, delete room
	if (false == master->SendPacket(packet)) {
		if (nullptr == tempRoom)
			return;

		this->RemoveRoom(tempRoom);
	}

}

void RoomManager::RemoveRoom(std::shared_ptr<Room> room) {
	m_roomNumberList.push(room->GetRoomNumber());
	m_roomList.remove(room);
}

std::shared_ptr<Room> RoomManager::FindRoom(Player* pplayer) {
	for (std::shared_ptr<Room> room : m_roomList) {
		if (room->GetRoomMaster() == pplayer) {
			return room;
		}
	}
}
