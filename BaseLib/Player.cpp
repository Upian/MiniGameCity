#include "Player.h"
#include "Log.h"
#include "BasePacket.h"


void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

/*void Player::HandlePacket(BufferInfo* bufInfo) {}*/

void Player::SetGamePacket(Buffer& buffer)
{
	if (buffer.Length() == 0) return;
	InGameBuf = buffer;
	RecvGameBuf = true;
}

Buffer* Player::GetGamePacket() {
	if (!RecvGameBuf) return nullptr;

	RecvGameBuf = false;
	return &InGameBuf;
}

void Player::PlayerInfoClear() {
	m_potionIndex = 0;
	m_room = nullptr;
	m_isRoomMaster = false;
	m_isReady = false;
}
