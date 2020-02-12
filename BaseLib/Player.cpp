#include "Player.h"
#include "Log.h"
#include "BasePacket.h"


void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

void Player::HandlePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type)
	{
	case BasePacketType::basePacketTypeGame: {
		InGameBuf = bufInfo;
		break;
	}
	}
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
