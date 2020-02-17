#include "RoomManager.h"
#include "Log.h"
#include "RoomPacket.h"

RoomManager::~RoomManager() {
	if (nullptr != m_roomWatcher)
		delete m_roomWatcher;
	m_roomWatcher = nullptr;
}

void RoomManager::HandleRoomPacket(Buffer & buffer, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	PacketTypeRoom type = (PacketTypeRoom)PacketTypeDeserial(buffer);
	
	switch (type) {
	case PacketTypeRoom::packetTypeRoomMakeRoomRequest: {
		RoomPacketMakeRoomRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketMakeRoom(packet, player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomRoomListRequest: {
		RoomPacketRoomListRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketRoomList(packet, player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomEnterRoomRequest: {
		RoomPacketEnterRoomRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketEnterRoom(packet, player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomLeaveRoomRequest: {
		RoomPacketLeaveRoomRequest packet;
		this->HandlePacketLeaveRoom(player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomRoomInfoRequest: {
		this->HandlePacketRoomInfo(player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomToggleReadyRequest: {
		this->HaldlePacketToggleReady(player);
		break;
	}
	case PacketTypeRoom::packetTypeRoomStartGameRequest: {
		this->HandlePacketStartGame(player);
		break;
	}
	default:
		Util::LoggingInfo("GameServer.log", "Recv wrong room packet ID: %d", type);
	}
}


void RoomManager::Initialize() {
	for (int i = m_maximumRoomNumber; i > 0; --i) {
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
				Util::LoggingInfo("0_Test.log", "Destroy room[%d] roomState: %d", room->GetRoomNumber(), room->GetRoomState());
				m_roomNumberList.push(room->GetRoomNumber());
				return true;
			}				
			return false;
		});
	}
}
/*Handle packet*/
void RoomManager::HandlePacketMakeRoom(RoomPacketMakeRoomRequest& packet, std::shared_ptr<Player> master) {
	if (nullptr == master)
		return;

	std::shared_ptr<Room> tempRoom;
	
	RoomPacketMakeRoomResponse responsePacket;
	
	//Check player number
	if ((packet.m_maximumPlayer < m_minimumPlayer) ||
		(packet.m_maximumPlayer > m_maximumPlayer)) {
		responsePacket.m_success = false;
		Util::LoggingInfo("", "Fail Make room");
	}	
	else if (RoomGameType::GameTypeNone == packet.m_gameType) {
		responsePacket.m_success = false;
		Util::LoggingInfo("Room.log", "Player[%d, %s] Does not check game type", master->GetGPID(), master->GetPlayerName().c_str());
	}
	//Success Make Room
	else if (nullptr == master->GetRoom()) {
			int roomNumber = m_roomNumberList.top();
			m_roomNumberList.pop();

			tempRoom = std::make_shared<Room>(roomNumber, packet.m_roomName, master, packet.m_maximumPlayer, packet.m_gameType);
			tempRoom->Initialize();

			if (0 < packet.m_password && packet.m_password < 10000) 
				tempRoom->SetPassword(packet.m_password);
			
			m_roomList.push_back(tempRoom);

			responsePacket.m_success = true;
			responsePacket.m_roomNumber = roomNumber;

			Util::LoggingDebug("Room.log", "Room [%s] number: %d, max: %d, password %s[%hd], game: %s",
				responsePacket.m_success ? "Success" : "Fail", responsePacket.m_roomNumber, packet.m_maximumPlayer,
				tempRoom->GetIsUsePassword() ? "True" : "False", tempRoom->GetPassword(),
				[&packet]()->const char*{
					switch (packet.m_gameType) {
					case RoomGameType::GameTypeTwentyQuestion:
						return "Twenty question";
					case RoomGameType::GameTypeBanKeyword:
						return "Ban keyword";
					case RoomGameType::GameTypeRelayNovel:
						return "Relay novel";
					case RoomGameType::GameTypeCatchMind:
						return "Catch mind";
				}
			});
	}
	//Alreade have room
	else{
		responsePacket.m_success = false;
		Util::LoggingInfo("", "Fail Already have room");
	}

	
	//send packet to room master
	//If send fails, delete room
	if (false == master->SendPacket(responsePacket)) {
		if (nullptr == tempRoom)
			return;

		this->RemoveRoom(tempRoom);
	}

}

void RoomManager::HandlePacketRoomList(RoomPacketRoomListRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	RoomPacketRoomListResponse responsePacket;
	if (true == m_roomList.empty()) {
		player->SendPacket(responsePacket);
		return;
	}

	int page = packet.m_page;
	int count = 0;
	for (auto room : m_roomList) {
		if (((page - 1)*m_maximumRoomOnePage) > count) {
			++count;
			continue;
		}
			
		if ((page * m_maximumRoomOnePage) <= count)
			break;

		Util::LoggingDebug("Room.log", "room number[%d] \t Room[%s], %d/%d, %s",
			room->GetRoomNumber(), room->GetRoomName().c_str(), room->GetPlayerCount(), room->GetMaxPlayerCount(), 
			room->GetIsUsePassword() ? "Yes" : "No");
		
		responsePacket.m_roomList.emplace_back(
			room->GetRoomNumber(), 
			room->GetCreatedTime(),
			room->GetPlayerCount(), 
			room->GetMaxPlayerCount(), 
			(RoomState::roomStateGaming == room->GetRoomState()),
			room->GetIsUsePassword(), 
			room->GetRoomName(),
			room->GetRoomGameType());
			
		++count;
	}

	player->SendPacket(responsePacket);
}

void RoomManager::HandlePacketEnterRoom(RoomPacketEnterRoomRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;

	std::shared_ptr<Room> tempRoom = nullptr;
	for (auto room : m_roomList) {
		if (packet.m_roomNumber == room->GetRoomNumber() && //Check room number
			packet.m_roomTime == room->GetCreatedTime()) { //Check room created time
			tempRoom = room;
			break;
		}
	}
	RoomPacketEnterRoomResponse responsePacket;
	responsePacket.m_errorType = ErrorTypeEnterRoom::errorTypeNone;
	responsePacket.m_isSuccess = false;
	//not exist room
	if (nullptr == tempRoom) { 
		responsePacket.m_errorType = ErrorTypeEnterRoom::errorTypeNotExistRoom;
		player->SendPacket(responsePacket);
		return;
	}

	responsePacket.m_errorType = tempRoom->PlayerEnterRoom(player, packet.m_password);
	if (ErrorTypeEnterRoom::errorTypeNone == responsePacket.m_errorType) {
		responsePacket.m_isSuccess = true;
		responsePacket.m_positionIndex = player->GetPositionIndex();
	}
		

	Util::LoggingDebug("Room.log", "Room status[%d]-[%s], players: %d\n",
		tempRoom->GetRoomNumber(), tempRoom->GetRoomName().c_str(), tempRoom->GetPlayerCount()); //#Test
	player->SendPacket(responsePacket);

	return;
}

void RoomManager::HandlePacketLeaveRoom(std::shared_ptr<Player> player) {
	auto room = player->GetRoom();
	if (nullptr == room) {
		return;
	}

	room->PlayerLeaveRoom(player);
}

void RoomManager::HandlePacketRoomInfo(std::shared_ptr<Player> player) {
	auto room = player->GetRoom();
	if (nullptr == room)
		return;

	room->UpdateRoomInfoToPlayer(player);
}

void RoomManager::HaldlePacketToggleReady(std::shared_ptr<Player> player) {
	auto room = player->GetRoom();
	if (nullptr == room)
		return;

	player->toggleReady();
	room->UpdateRoomInfoToAllPlayers();
}

void RoomManager::HandlePacketStartGame(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	auto room = player->GetRoom();
	if (nullptr == room)
		return;

	if (false == player->GetIsRoomMaster()) //Check room master
		return;
	if (room->GetRoomMaster() != player) //Check room master
		return;

	room->StartGame();
}

void RoomManager::RemoveRoom(std::shared_ptr<Room> room) {
	m_roomNumberList.push(room->GetRoomNumber());
	m_roomList.remove(room);
}

std::shared_ptr<Room> RoomManager::FindRoomByPlayer(std::shared_ptr<Player> pplayer) {
	for (std::shared_ptr<Room> room : m_roomList) {
		if (true == room->IsExistPlayer(pplayer)) {
			return room;
		}
	}
	return nullptr;
}
