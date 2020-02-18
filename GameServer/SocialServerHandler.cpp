#include "SocialServerHandler.h"
#include "SocialGamePacket.h"
#include "SocialServerPacket.h"
#include "GameServer.h"
#include "Player.h"
#include "Log.h"
#include "Room.h"


void SocialServerHandler::Initialize() {
	m_gameServer = GameServer::GetServer();
	this->ConnectToServer(
		Util::GetConfigToString("GameServer.ini", "Network", "SocialServerIP", "127.0.0.1").c_str(),
		Util::GetConfigToInt("GameServer.ini", "Network", "SocialServerPort", 20000)
	);
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
	case PacketTypeSocialClient::packetTypeSocialChatFriendRequest: {
		SocialGamePacketChatFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketChatFriendRequest(packet, player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialChatGuildRequest: {

		break;
	}
	case PacketTypeSocialClient::packetTypeSocialAddFriendRequest: {
		SocialGamePacketAddFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketAddFriendRequest(packet, player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialConfirmFriendRequest: {
		SocialGamePacketConfirmFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketConfirmFriendRequest(player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialAcceptFriendRequest: {
		SocialGamePacketAcceptFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketAcceptFriendRequest(packet, player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialFriendListRequest: {
		SocialGamePacketFriendListRequest packet;
		this->HandlePacketFriendListRequest(player);
		break;
	}
	case PacketTypeSocialClient::packetTypeSocialDeleteFriendRequest: {
		SocialGamePacketDeleteFriendRequest packet;
		packet.Deserialize(buffer);
		this->HandlePacketDeleteFriendRequest(packet, player);
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
	if (BasePacketType::basePacketTypeGameToSocialServer != type)
		return;
	
	PacketTypeSocialServer socialType = (PacketTypeSocialServer)PacketTypeDeserial(buffer);
	switch (socialType) {
	case PacketTypeSocialServer::addFriendResponse: {
		SocialPacketServerAddFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketAddFriendResponse(packet, pplayer);
		break;
	}
	case PacketTypeSocialServer::deleteFriendResponse: {
		SocialPacketServerDeleteFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketDeleteFriendResponse(packet, pplayer);
		break;
	}
	case PacketTypeSocialServer::confirmFriendResponse: {
		SocialPacketServerConfirmFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketConfirmFriendResponse(packet, pplayer);
		break;
	}
	case PacketTypeSocialServer::acceptFriendResponse: {
		SocialPacketServerAcceptFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketAcceptFriendResponse(packet, pplayer);
		break;
	}
	case PacketTypeSocialServer::friendListResponse: {
		SocialPacketServerFriendListResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketFriendListResponse(packet, pplayer);
		break;
	}
	case PacketTypeSocialServer::chatFriendResponse: {
		SocialPacketServerChatFriendResponse packet;
		packet.Deserialize(buffer);
		auto pplayer = this->GetPlayer(packet.m_gpid);
		this->HandlePacketChatFriendResponse(packet, pplayer);
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
void SocialServerHandler::HandlePacketChatFriendRequest(SocialGamePacketChatFriendRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;

	SocialPacketServerChatFriendRequest sendPacket;
	sendPacket.m_srcGpid = player->GetGPID();
	sendPacket.m_destName = packet.m_destName;
	sendPacket.m_message = packet.m_message;
	this->SendPacketToServer(sendPacket);
}
void SocialServerHandler::HandlePacketAddFriendRequest(SocialGamePacketAddFriendRequest& packet, std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	
	SocialPacketServerAddFriendRequest sendPacket;
	sendPacket.m_srcGpid = player->GetGPID();/*player->GetGPID();*/
	sendPacket.m_destName = packet.m_destName;
	this->SendPacketToServer(sendPacket);
}

void SocialServerHandler::HandlePacketConfirmFriendRequest(std::shared_ptr<Player> player) {
	if (nullptr == player)
		return;
	//Game server to social server
	SocialPacketServerConfirmFriendRequest sendPacket;
	sendPacket.m_gpid = player->GetGPID();
	
	this->SendPacketToServer(sendPacket);
}

void SocialServerHandler::HandlePacketAcceptFriendRequest(SocialGamePacketAcceptFriendRequest& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialPacketServerAcceptFriendRequest sendPacket;
	sendPacket.m_gpid = pplayer->GetGPID();
	sendPacket.m_isAccept = packet.m_isAccept;
	sendPacket.m_name = packet.m_name;
	
	this->SendPacketToServer(sendPacket);
}
void SocialServerHandler::HandlePacketFriendListRequest(std::shared_ptr<Player> player) {
	SocialPacketServerFriendListRequest packet;
	packet.m_gpid = player->GetGPID();

	this->SendPacketToServer(packet);
}
void SocialServerHandler::HandlePacketDeleteFriendRequest(SocialGamePacketDeleteFriendRequest& packet, std::shared_ptr<Player> player) {
	SocialPacketServerDeleteFriendRequest sendPacket;
	sendPacket.m_gpid = player->GetGPID();
	sendPacket.m_name = packet.m_name;

	this->SendPacketToServer(sendPacket);
}
///////////////////////////////////////////////////////////////
void SocialServerHandler::HandlePacketAddFriendResponse(SocialPacketServerAddFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;

	SocialGamePacketAddFriendResponse responsePacket;
	responsePacket.m_success = packet.m_success;
	responsePacket.m_errorCode = packet.m_errorType;

	pplayer->SendPacket(responsePacket);
}

void SocialServerHandler::HandlePacketDeleteFriendResponse(SocialPacketServerDeleteFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	SocialGamePacketDeleteFriendResponse responsePacket;
	responsePacket.m_isSuccess = packet.m_isSuccess;

	pplayer->SendPacket(responsePacket);
}

void SocialServerHandler::HandlePacketConfirmFriendResponse(SocialPacketServerConfirmFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;
	SocialGamePacketConfirmFriendResponse responsePacket;
	responsePacket.m_names = packet.m_names;
	
	pplayer->SendPacket(responsePacket);
}

void SocialServerHandler::HandlePacketAcceptFriendResponse(SocialPacketServerAcceptFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;
	
	SocialGamePacketAcceptFriendResponse responsePacket;
	responsePacket.m_errorCode = packet.m_errorCode;

	pplayer->SendPacket(responsePacket);
}

void SocialServerHandler::HandlePacketFriendListResponse(SocialPacketServerFriendListResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;
	SocialGamePacketFriendListResponse responsePacket;

	responsePacket.m_friends.swap(packet.m_names);

	pplayer->SendPacket(responsePacket);
}

void SocialServerHandler::HandlePacketChatFriendResponse(SocialPacketServerChatFriendResponse& packet, std::shared_ptr<Player> pplayer) {
	if (nullptr == pplayer)
		return;
	SocialGamePacketChatFriendResponse responsePacket;

	responsePacket.m_name = packet.m_name;
	responsePacket.m_message = packet.m_message;
	responsePacket.m_isSender = packet.m_isSender;

	pplayer->SendPacket(responsePacket);
}

std::shared_ptr<Player> SocialServerHandler::GetPlayer(GPID gpid) { 
	return m_gameServer->GetPlayerManager().FindPlayer(gpid); 
}
