#include "RoomManager.h"
#include "Room.h"
#include "Player.h"
#include "Log.h"
#include "InGameLibrary.h"
#include "ErrorType.h"
Room::Room(int roomNumber, std::string roomName, std::shared_ptr<Player> master, int maxPlayer, RoomGameType gameType) :
	m_roomName(roomName),
	m_maximumPlayer(maxPlayer),
	m_roomMaster(master),
	m_roomNumber(roomNumber),
	m_roomState(RoomState::roomStateWait),
	m_gameType(gameType) {
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
	
	m_game = this->SetGame();

	m_roomMaster->SetIsRoomMaster(true);
	this->PlayerEnterRoom(m_roomMaster);
}

void Room::StartGame() {
	RoomPacketStartGameResponse packet;
	packet.m_isSuccess = false;
	packet.m_errorCode == ErrorTypeStartGame::errorTypeStartGameNone;

	if (nullptr == m_game)
		packet.m_errorCode = ErrorTypeStartGame::errorTypeStartGameNotHaveGame;
	else if (RoomState::roomStateWait != m_roomState)
		packet.m_errorCode = ErrorTypeStartGame::errorTypeStartGameRoomIsNotWaitingGame;
	else if (nullptr != m_inGameThread)
		packet.m_errorCode = ErrorTypeStartGame::errorTypeStartGameAlreadyStartGame;
	else if (false == this->CheckAllPlayerIsReady())//Check all player ready
		packet.m_errorCode = ErrorTypeStartGame::errorTypeStartGameReady;

	if (packet.m_errorCode == ErrorTypeStartGame::errorTypeStartGameNone)
		packet.m_isSuccess = true;

	m_roomPlayerManager.SendToAllPlayers(packet);
	if (false == packet.m_isSuccess) {
		Util::LoggingInfo("Room.log", "Can not start room[%d, %s] errorCode: %d", m_roomNumber, m_roomName.c_str(), packet.m_errorCode);
		return;
	}
		
	Util::LoggingInfo("Room.log", "---Room[%d]is successfully start---", m_roomNumber);
	for (auto p : m_roomPlayerManager.GetPlayerList()) {
		Util::LoggingInfo("Room.log", "player[%d, %s], %s", p->GetGPID(), p->GetPlayerName().c_str(), p->GetIsRoomMaster() ? "is RoomMaster" : "Player");
	}
	
	m_inGameThread = new std::thread([this]()->void {
		m_roomState = RoomState::roomStateGaming;
		m_roomPlayerManager.SetAllPlayerState(PlayerState::playerStatePlayGame);
		m_game(m_roomPlayerManager);
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
bool Room::CheckAllPlayerIsReady() {
	for (auto player : m_roomPlayerManager.GetPlayerList()) {
		if (false == player->GetIsReady())
			return false;
	}
	return true;
}
std::function<void(PlayerManager&)> Room::SetGame() {
	switch (m_gameType) {
	case RoomGameType::GameTypeTwentyQuestion: {
		return [this](PlayerManager& players)->void {this->m_gameLib.TwentyQuestionGame(players); };
	}
	case RoomGameType::GameTypeRelayNovel: {
		return [this](PlayerManager& players)->void {this->m_gameLib.RelayNovelGame(players); };
	}
	case RoomGameType::GameTypeBanKeyword: {
		return [this](PlayerManager& players)->void {this->m_gameLib.BanKeywordGame(players); };
	}
	case RoomGameType::GameTypeCatchMind: {
		return [this](PlayerManager& players)->void {this->m_gameLib.CatchMindGame(players); };
	}
	case RoomGameType::GameTypeNone: break;
	default: {
		Util::LoggingError("Room.h", "Game type error: %d", m_gameType);
	}
	}
	return nullptr;
}
#pragma region operator

#pragma endregion
