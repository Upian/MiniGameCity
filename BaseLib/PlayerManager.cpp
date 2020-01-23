#include "PlayerManager.h"
#include "Player.h"
#include "Log.h"
#include <memory>

PlayerManager::PlayerManager() {

}
PlayerManager::~PlayerManager() {

}

void PlayerManager::PlayerLogin(SOCKET socket) {
	m_playerList.emplace_back(new Player(socket));
}

void PlayerManager::PlayerLogout(GPID key) {
	auto pPlayer = this->FindPlayer(key);
	
	m_playerList.remove(pPlayer);
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