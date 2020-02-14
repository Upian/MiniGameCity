#include "Player.h"
#include "Log.h"
#include "BasePacket.h"


void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

/*void Player::HandlePacket(BufferInfo* bufInfo) {}*/

void Player::SetGamePacket(BufferInfo* buffer)
{
	if (buffer == nullptr) return;
	InGameBuf = buffer;
}

BufferInfo* Player::GetGamePacket() {
	if (InGameBuf == nullptr) 
		return nullptr;

	BufferInfo* temp = InGameBuf;
	InGameBuf = nullptr;
	return temp;
}

void Player::PlayerInfoClear() {
	m_potionIndex = 0;
	m_room = nullptr;
	m_isRoomMaster = false;
	m_isReady = false;
}
