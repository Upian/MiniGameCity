#include "Player.h"
#include "Log.h"
#include "BasePacket.h"


Player::Player(GPID gpid, SessionID session) :
	m_gamePlayerId(gpid),
	m_sessionID(session)
{
}

void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

/*void Player::HandlePacket(BufferInfo* bufInfo) {}*/

void Player::PlayerInfoClear() {
	m_potionIndex = 0;
	m_room = nullptr;
	m_isRoomMaster = false;
	m_isReady = false;
}
