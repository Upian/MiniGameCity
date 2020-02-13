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

//Client to game server
void SocialServerHandler::HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player) { //server to client
	if (nullptr == player)
		return;

	PacketTypeSocialClient type = (PacketTypeSocialClient)PacketTypeDeserial(buffer);

	switch (type) {
	case PacketTypeSocialClient::packetTypeSocialChatNormalRequest: {
		SocialGamePacketChatNormalRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketChatNormalRequest(packet, player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialAddFriendRequest: {
		SocialGamePacketAddFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketAddFriendRequest(packet, player);
		break;
	}
	default:
		Util::LoggingError("Social.log", "Un defined packet error. packet type[%d]", type);
	}
}

void SocialServerHandler::UpdatePlayerInfoAtLogin(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialPacketServerUpdatePlayerLogin packet;
	packet.m_gpid = pplayer->GetGPID();
	packet.m_name = pplayer->GetPlayerName();

	this->SendPacketToServer(packet);
}

void SocialServerHandler::UpdatePlayerInfoAtLogout(std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialPacketServerUpdatePlayerLogout packet;
	packet.m_gpid = pplayer->GetGPID();

	this->SendPacketToServer(packet);
}

//Social server to client
void SocialServerHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);
	if (BasePacketType::basePacketTypeSocialServer != type)
		return;
	
	PacketTypeSocialServer socialType = (PacketTypeSocialServer)PacketTypeDeserial(buffer);
	switch (socialType) {
	case PacketTypeSocialServer::addFriendResponse: {
		SocialPacketServerAddFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = m_gameServer->GetPlayerManager().FindPlayer(packet.m_gpid);
		this->HandlePacketAddFriendResponse(packet, pplayer);
		break;
	}
	default:break;
	}
}

void SocialServerHandler::HandlePacketChatNormalRequest(SocialGamePacketChatNormalRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialGamePacketChatNormalResponse responsePacket;
	responsePacket.m_name = player->GetPlayerName();
	responsePacket.m_message = packet.m_message;

	switch (player->GetPlayerState()) {
	case PlayerState::playerStateLobby: {
		if (nullptr == m_gameServer)
			m_gameServer = GameServer::GetServer();
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
void SocialServerHandler::HandlePacketAddFriendRequest(SocialGamePacketAddFriendRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialPacketServerAddFriendRequest sendPacket;
	sendPacket.m_srcGpid = player->GetGPID();/*player->GetGPID();*/
	sendPacket.m_destName = packet.m_destName;
	Util::LoggingDebug("", "Send packet to social server [%d] --> [%s]", sendPacket.m_srcGpid, sendPacket.m_destName.c_str());
	this->SendPacketToServer(sendPacket);
}

void SocialServerHandler::HandlePacketAddFriendResponse(SocialPacketServerAddFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialGamePacketAddFriendResponse responsePacket;
	responsePacket.m_success = packet.m_success;
	responsePacket.m_errorCode = responsePacket.m_errorCode;

	pplayer->SendPacket(responsePacket);
}
