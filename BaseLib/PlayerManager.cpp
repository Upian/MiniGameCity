#include "PlayerManager.h"
#include "Player.h"
#include "Log.h"
#include <memory>

void PlayerManager::InsertPlayer(SOCKET socket) {
	if (nullptr != this->FindPlayerBySocket(socket)) //already exist
		return;

	std::shared_ptr<Player> tempPlayer = std::make_shared<Player>(socket);
	tempPlayer->Initialize();
	
	m_playerList.push_back(tempPlayer);
}

void PlayerManager::RemovePlayer(GPID key) {
	auto pPlayer = this->FindPlayer(key);
	if (nullptr == pPlayer)
		return;
	m_playerList.remove(pPlayer);
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

void PlayerManager::SendToPlayers(BasePacket& packet) {
	for (std::shared_ptr<Player> k : m_playerList) {
		if(nullptr == k) continue;

		k->SendPacket(packet);
		//WSAsend
	}
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