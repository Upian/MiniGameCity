#include "SocialServerHandler.h"
#include "SocialGamePacket.h"
#include "SocialServerPacket.h"
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
		SocialGamePacketChatNormalRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketChatNormal(packet, player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialAddFriendRequest: {
		SocialGamePacketAddFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketAddFriend(packet, player);
	}
	default:
		Util::LoggingError("Social.log", "Un defined packet error. packet type[%d]", type);
	}
}

void SocialServerHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);
	if (BasePacketType::basePacketTypeSocialServer != type)
		return;
	
	PacketTypeSocialServer socialType = (PacketTypeSocialServer)PacketTypeDeserial(buffer);
	switch (socialType) {
	case PacketTypeSocialServer::addFriendResponse: {
		Util::LoggingInfo("", "TTTETST Recv packet");
		break;
	}
	default:break;
	}
}

void SocialServerHandler::HandlePacketChatNormal(SocialGamePacketChatNormalRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialGamePacketChatNormalResponse responsePacket;
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
void SocialServerHandler::HandlePacketAddFriend(SocialGamePacketAddFriendRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialPacketServerAddFriendRequest sendPacket;
	sendPacket.m_src = player->GetGPID();/*player->GetGPID();*/
	sendPacket.m_destName = packet.m_destName;
	Util::LoggingDebug("", "Send packet to social server [%d] --> [%s]", sendPacket.m_src, sendPacket.m_destName.c_str());
	this->SendPacketToServer(sendPacket);
}
