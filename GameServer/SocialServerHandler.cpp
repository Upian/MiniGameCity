#include "SocialServerHandler.h"
#include "SocialPacket.h"
#include "GameServer.h"
#include "Player.h"
#include "Log.h"
#include "Room.h"


void SocialServerHandler::Initialize() {
	m_gameServer = GameServer::GetServer();
}

void SocialServerHandler::HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player) { //server to client
	if (nullptr == player)
		return;

	PacketTypeSocialClient type = (PacketTypeSocialClient)PacketTypeDeserial(buffer);

	switch (type) {
	case PacketTypeSocialClient::packetTypeSocialChatNormalRequest: {
		SocialPacketChatNormalRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketChatNormal(packet, player);
		break;
	}
	default:
		Util::LoggingError("Social.log", "Un defined packet error. packet type[%d]", type);
	}
}

void SocialServerHandler::HandlePacketChatNormal(SocialPacketChatNormalRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialPacketChatNormalResponse responsePacket;
	responsePacket.m_name = player->GetPlayerName();
	responsePacket.m_message = packet.m_message;

	switch (player->GetPlayerState()) {
	case PlayerState::playerStateLobby: {
		m_gameServer->GetPlayerManager().SendToLobbyPlayers(responsePacket);
		break;
	}
	case PlayerState::playerStateRoom: {
		auto room = player->GetRoom();
		if (nullptr == room)
			return;

		room->GetPlayerManager().SendToAllPlayers(responsePacket);
		break;
	}
	default:
		break;

	}
}
