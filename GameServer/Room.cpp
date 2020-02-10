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
	m_createdTime = time(NULL);
}

Room::~Room() {
	if (nullptr != m_inGameThread) {
		if (true == m_inGameThread->joinable())
			m_inGameThread->join();
		delete m_inGameThread;
	}
		
	m_inGameThread = nullptr;
}

void Room::Initialize() {
	for (int i = m_maximumPlayer; i > 0; --i) 
		m_playerPosionStack.push(i);
	
	m_roomMaster->SetIsRoomMaster(true);
	this->PlayerEnterRoom(m_roomMaster);
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

ErrorTypeEnterRoom Room::PlayerEnterRoom(std::shared_ptr<Player> player, __int16 password /* = 0 */) {
	if (nullptr == player) 
		return ErrorTypeEnterRoom::errorTypePlayerLogout;

	ErrorTypeEnterRoom errorType = ErrorTypeEnterRoom::errorTypeNone;
	//Wrong password
	if (true == m_isUsePassword && false == this->CheckPassword(password)) {
		errorType = ErrorTypeEnterRoom::errorTypeWrongPassword;
		Util::LoggingError("Room.log", "Error %d - Wrong password -> Room[%hd], Player[%hd]", 
			player->GetGPID(), this->GetPassword(), password);
	}
	//Already included in another room
	else if (PlayerState::playerStateRoom == player->GetPlayerState()) {
		errorType = ErrorTypeEnterRoom::errorTypeAlreadyIncluded;
		Util::LoggingError("Room.log", "Error %d - Already included in another room[]", 
			player->GetGPID());
	}
	//already start 
	else if (true == this->IsRoomStateGaming()) {
		errorType = ErrorTypeEnterRoom::errorTypeGameStart;
		Util::LoggingError("Room.log", "Error Already Game start");
	}	
	//can not enter room
	else if (true == this->CheckIsRoomIsFull() ||
		0 >= m_playerPosionStack.size()) {
		errorType = ErrorTypeEnterRoom::errorTypeMaxPlayer;
		Util::LoggingError("Room.log", "Error Full room %d/%d  stackSize[%d]", 
			m_roomPlayerManager.GetPlayerCount(), m_maximumPlayer, m_playerPosionStack.size());
	}

	if (ErrorTypeEnterRoom::errorTypeNone != errorType) {
		return errorType;
	}

	player->SetPlayerState(PlayerState::playerStateRoom);
	player->SetPositionIndex(m_playerPosionStack.top());
	player->SetIsReady(false);
	m_playerPosionStack.pop();
	m_roomPlayerManager.InsertPlayer(player);
	player->SetRoom(shared_from_this());

	this->UpdateRoomInfoToAllPlayers();

	return errorType;
}

void Room::PlayerLeaveRoom(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	RoomPacketLeaveRoomResponse leavePacket;
	leavePacket.m_isSuccess = true;

	m_roomPlayerManager.RemovePlayer(player);//remove player
	
	m_playerPosionStack.push(player->GetPositionIndex());
	
	if (true == m_roomPlayerManager.IsPlayerEmpty()) {
		m_roomState = RoomState::roomStateNone;
	}
	else if (m_roomMaster == player) {//If the player is room master
		player->SetIsRoomMaster(false);
		m_roomMaster = m_roomPlayerManager.GetPlayerList().front(); //
		m_roomMaster->SetIsRoomMaster(true);
	}
	//reset player info
	player->SetPlayerState(PlayerState::playerStateLobby);
	player->PlayerInfoClear();

	player->SendPacket(leavePacket);
	this->UpdateRoomInfoToAllPlayers();

	return;
}

void Room::UpdateRoomInfoToAllPlayers() {
	RoomPacketRoomInfoResponse packet;
	for (auto p : m_roomPlayerManager.GetPlayerList()) {
		packet.m_players.emplace_back(
			p->GetPositionIndex(),
			p->GetIsRoomMaster(),
			p->GetPlayerName(),
			0,
			p->GetIsReady()
		);
	}

	m_roomPlayerManager.SendToAllPlayers(packet);
}

void Room::UpdateRoomInfoToPlayer(std::shared_ptr<Player> player) {
	RoomPacketRoomInfoResponse packet;
	for (auto p : m_roomPlayerManager.GetPlayerList()) {
		packet.m_players.emplace_back(
			p->GetPositionIndex(),
			p->GetIsRoomMaster(),
			p->GetPlayerName(),
			0,
			p->GetIsReady()
		);
	}

	player->SendPacket(packet);
}

void Room::SetPassword(__int16 password) {
	if (10000 < password)
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
