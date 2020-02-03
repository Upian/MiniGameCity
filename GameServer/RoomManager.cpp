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

void RoomManager::HandleMakeRoom(RoomPacketMakeRoomRequest& packet, std::shared_ptr<Player> master) {
	if (nullptr == master)
		return;

	std::shared_ptr<Room> tempRoom;
	
	RoomPacketMakeRoomResponse responsePacket;
	//Check player number

	if ((packet.m_maxPlayer < m_minPlayer) ||
		(packet.m_maxPlayer > m_maxPlayer)) {
		responsePacket.m_success = false;
		Util::LoggingInfo("", "Fail Make room");
	}	
	else if (nullptr == this->FindRoomByPlayer(master)) {
			int roomNumber = m_roomNumberList.top();
			m_roomNumberList.pop();

			tempRoom = std::make_shared<Room>(packet.m_roomName, packet.m_maxPlayer, master, roomNumber);
			m_roomList.push_back(tempRoom);

			responsePacket.m_success = true;
			responsePacket.m_roomNumber = roomNumber;

			Util::LoggingInfo("", "Success Make room - number[%d]", roomNumber);
		
	}
	else {
		responsePacket.m_success = false;
		Util::LoggingInfo("", "Fail Already have room");
	}

	
	//send packet to room master
	//If send fails, delete room
	printf("Room [%s] number: %d, max: %d\n", responsePacket.m_success ? "Success" : "Fail", responsePacket.m_roomNumber, packet.m_maxPlayer);
	if (false == master->SendPacket(responsePacket)) {
		if (nullptr == tempRoom)
			return;

		this->RemoveRoom(tempRoom);
	}

}

void RoomManager::RemoveRoom(std::shared_ptr<Room> room) {
	m_roomNumberList.push(room->GetRoomNumber());
	m_roomList.remove(room);
}

std::shared_ptr<Room> RoomManager::FindRoomByPlayer(std::shared_ptr<Player> pplayer) {
	for (std::shared_ptr<Room> room : m_roomList) {
		if (room->GetRoomMaster() == pplayer) {
			return room;
		}
	}
	return nullptr;
}
