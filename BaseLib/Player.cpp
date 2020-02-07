#include "Player.h"
#include "Log.h"


void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

void Player::HandlePacket(BufferInfo* bufInfo) {

}

void Player::PlayerInfoClear() {
	m_potionIndex = 0;
	m_room = nullptr;
	m_isRoomMaster = false;
	m_isReady = false;
}
