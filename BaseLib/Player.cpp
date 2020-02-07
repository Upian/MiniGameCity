#include "Player.h"
#include "Log.h"


void Player::Initialize() {
	//Generate GPID
	m_playerState = PlayerState::playerStateLobby;
}

void Player::HandlePacket(BufferInfo* bufInfo) {

}
