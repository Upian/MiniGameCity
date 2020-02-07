#include "SocialManager.h"
#include "SocialPacket.h"
#include "GameServer.h"
#include "Player.h"
#include "Log.h"
#include "Room.h"


void SocialManager::Initialize() {
	m_gameServer = GameServer::GetServer();
}

void SocialManager::HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;

	PacketTypeSocial type = (PacketTypeSocial)PacketTypeDeserial(buffer);

	switch (type) {
	case PacketTypeSocial::packetTypeSocialChatNormalRequest: {
		SocialPacketChatNormalRequest packet;

		this->HandlePacketChatNormal(packet, player);
		break;
	}
	default:
		Util::LoggingError("Social.log", "Un defined packet error. packet type[%d]", type);
	}
}

void SocialManager::HandlePacketChatNormal(SocialPacketChatNormalRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;


//	switch (player->GetPlayerState()) {
//	case PlayerState::playerStateLobby: {
//		m_gameServer->GetPlayerManager().SendToLobbyPlayers(packet);
//	}
//	case PlayerState::playerStateRoom: {
//		auto room = player->GetRoom();
//		if (nullptr == room)
//			return;
//
//		room->GetPlayerManager().SendToAllPlayers(packet);
//	}
//	default:
//
//	}
}
