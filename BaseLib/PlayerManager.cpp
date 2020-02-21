#include <memory>
#include "PlayerManager.h"
#include "Player.h"
#include "Log.h"
#include "BasePacket.h"

void PlayerManager::InsertPlayer(std::shared_ptr<Player> player) {
	if (true == this->IsExistPlayer(player)) //already exist
		return;

	m_playerList.push_back(player); 
}

std::shared_ptr<Player> PlayerManager::InsertPlayer(SOCKET socket) {
	if (nullptr != this->FindPlayerBySocket(socket)) //already exist
		return nullptr;

	std::shared_ptr<Player> tempPlayer = std::make_shared<Player>(socket);
	tempPlayer->Initialize();
	
	m_playerList.push_back(tempPlayer);
	return tempPlayer;
}

void PlayerManager::RemovePlayer(GPID key) {
	auto itPlayer = m_playerList.begin();
	for (itPlayer; itPlayer != m_playerList.end(); ++itPlayer) {
		if (key == (*itPlayer)->GetGPID()) {
			itPlayer = m_playerList.erase(itPlayer);
			return;
		}
	}
}

void PlayerManager::RemovePlayer(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	m_playerList.remove(player);
}

void PlayerManager::PlayerDisconnect(SOCKET key) {
	auto pPlayer = this->FindPlayerBySocket(key);
	if (nullptr == pPlayer)
		return;
	m_playerList.remove(pPlayer);
}
void PlayerManager::PlayerDisconnect(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;
	m_playerList.remove(pplayer);
}

void PlayerManager::SendToAllPlayers(BasePacket& packet) {
	Buffer buffer = packet.Serialize();

	for (std::shared_ptr<Player> p : m_playerList) {
		if(nullptr == p) 
			continue;

		p->SendPacket(buffer);
	}
}

void PlayerManager::SendToLobbyPlayers(BasePacket& packet) {
	Buffer buffer = packet.Serialize();
	for (auto p : m_playerList) {
		if(nullptr == p) 
			continue;

		if (PlayerState::playerStateLobby != p->GetPlayerState())
			continue;

		p->SendPacket(buffer);
	}
}

void PlayerManager::SendToGuildPlayers(BasePacket & packet) {
}

void PlayerManager::SetAllPlayerState(PlayerState state) {
	for (auto p : m_playerList) {
		if(nullptr == p)
			continue;

		p->SetPlayerState(state);
	}
}

bool PlayerManager::IsExistPlayer(std::shared_ptr<Player> player) {
	for (auto p : m_playerList) {
		if (player == p)
			return true;
	}
	return false;
}

std::shared_ptr<Player> PlayerManager::FindPlayer(GPID playerId) {
	for (std::shared_ptr<Player> ptr : m_playerList) {
		if (nullptr == ptr)
			continue;

		if (playerId == ptr->GetGPID()) 
			return ptr;
	}
	return nullptr;
}

std::shared_ptr<Player> PlayerManager::FindPlayerBySocket(SOCKET socket) {
	for (std::shared_ptr<Player> ptr : m_playerList) {
		if (nullptr == ptr)
			continue;

		if (socket == ptr->GetSocket())
			return ptr;
	}
	return nullptr;
}

std::shared_ptr<Player> PlayerManager::FindPlayerByName(std::string name) {
	for (std::shared_ptr<Player> ptr : m_playerList) {
		if (nullptr == ptr)
			continue;

		if (name == ptr->GetName())
			return ptr;
	}
	return nullptr;
}

std::shared_ptr<Player> PlayerManager::PreLoadClient(GPID gpid, SessionID session) {
	auto preLoad = std::make_shared<Player>(gpid, session);
	m_preLoadClientList.push_back(preLoad);
	preLoad->Initialize();
}
