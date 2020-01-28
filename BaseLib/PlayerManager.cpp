#include "PlayerManager.h"
#include "Player.h"
#include "Log.h"
#include <memory>


void PlayerManager::RemovePlayer(GPID key) {
	Player* pPlayer = this->FindPlayer(key);

	m_playerList.remove(pPlayer);
}

void PlayerManager::RemovePlayer(Player* player) {
	m_playerList.remove(player);
}

void PlayerManager::PlayerDisconnect(SOCKET key) {
	auto pPlayer = this->FindPlayerBySocket(key);

	m_playerList.remove(pPlayer);
}

void PlayerManager::SendToPlayers(BasePacket* packet) {
	if (nullptr == packet) return;

	for (Player* k : m_playerList) {
		if(nullptr == k) continue;

		//WSAsend
	}
}

Player* PlayerManager::FindPlayer(GPID playerId) {
	for (Player* k : m_playerList) {
		if (nullptr == k)
			continue;

		if (playerId == k->GetGPID()) 
			return k;
	}

	return nullptr;
}

Player* PlayerManager::FindPlayerBySocket(SOCKET socket) {
	for (Player* k : m_playerList) {
		if (nullptr == k)
			continue;

		if (socket == k->GetSocket())
			return k;
	}

	return nullptr;
}