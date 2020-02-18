#include "SocialServer.h"
#include "BasePacket.h"
#include "SocialServerPacket.h"
#include "SocialPlayer.h"
SocialServer::SocialServer() {}
SocialServer::~SocialServer() {}

void SocialServer::InitializeSocialServer() {

}

void SocialServer::HandleAcceptClient(SOCKET gameServerSocket) {
	m_gameServers.InsertServer(gameServerSocket);
	
	Util::LoggingInfo("0_test.log", "m_gameserver Size: %d", m_gameServers.Size());
}

void SocialServer::HandleDisconnectClient(SOCKET gameServerSocket) {
	m_gameServers.RemoveServer(gameServerSocket);

}

void SocialServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case BasePacketType::basePacketTypeGameToSocialServer: {
		this->HandleBaseSocialPacket(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong base packet ID: %d", type);
		break;
	}
		
	}
}

void SocialServer::HandleBaseSocialPacket(BufferInfo* bufInfo) {
	PacketTypeSocialServer type = (PacketTypeSocialServer)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case PacketTypeSocialServer::updatePlayerLogin: { 
		SocialPacketServerUpdatePlayerLogin packet;
		packet.Deserialize(bufInfo->buffer);
		this->HandleUpdatePlayerLogin(packet, m_gameServers.FindServerBySocket(bufInfo->socket));
		break;
	}
	case PacketTypeSocialServer::updatePlayerLogout: {
		SocialPacketServerUpdatePlayerLogout packet;
		packet.Deserialize(bufInfo->buffer);
		this->HandleUpdatePlayerLogout(packet, m_gameServers.FindServerBySocket(bufInfo->socket));
		break;
	}
	case PacketTypeSocialServer::addFriendRequest: {
		SocialPacketServerAddFriendRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto srcPlayer = m_socialPlayerManager.FindSocialPlayer(packet.m_srcGpid);
//		auto destPlayer = m_socialPlayerManager.FindSocialPlayer(packet.m_destName);
		m_friendManager.HandleAddFriendRequest(srcPlayer, packet.m_destName);
		break;
	}
	case PacketTypeSocialServer::confirmFriendRequest: {
		SocialPacketServerConfirmFriendRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto pplayer = m_socialPlayerManager.FindSocialPlayer(packet.m_gpid);
		m_friendManager.HandleConfirmFriendRequest(pplayer);
		break;
	}
	case PacketTypeSocialServer::acceptFriendRequest: {
		SocialPacketServerAcceptFriendRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto srcPlayer = m_socialPlayerManager.FindSocialPlayer(packet.m_gpid);
		m_friendManager.HandleAcceptFriendReequest(srcPlayer, packet.m_name, packet.m_isAccept);
		break;
	}
	case PacketTypeSocialServer::friendListRequest: {
		SocialPacketServerFriendListRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto player = m_socialPlayerManager.FindSocialPlayer(packet.m_gpid);
		m_friendManager.HandleFriendListRequest(player);
		break;
	}
	case PacketTypeSocialServer::deleteFriendRequest: {
		SocialPacketServerDeleteFriendRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto player = m_socialPlayerManager.FindSocialPlayer(packet.m_gpid);
		m_friendManager.HandleDeleteFriendRequest(player, packet.m_name);
		break;
	}
	case PacketTypeSocialServer::chatFriendRequest: {
		SocialPacketServerChatFriendRequest packet;
		packet.Deserialize(bufInfo->buffer);
		auto player = m_socialPlayerManager.FindSocialPlayer(packet.m_srcGpid);
		m_friendManager.HandleChatFriendRequest(player, packet.m_destName, packet.m_message);
		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong soical server packet ID: %d", type);
		break;
	}
	}
}

void SocialServer::HandleUpdatePlayerLogin(SocialPacketServerUpdatePlayerLogin& packet, std::shared_ptr<ClntServer> server) {
	if (nullptr == server) 
		return;
	printf("NEW PLAYER TRY LOGIN %d - %s\n", packet.m_gpid, packet.m_name.c_str());
	std::shared_ptr<SocialPlayer> pplayer = m_socialPlayerManager.InsertPlayer(packet.m_gpid, server);
	if (nullptr == pplayer)
		return;
	printf("NEW PLAYER LOGIN %d - %s\n", pplayer->GetGPID(), packet.m_name);

	pplayer->SetName(packet.m_name); //#Test
	//#DatabaseLoad
	this->LoadPlayerSocialData(pplayer);
}

void SocialServer::HandleUpdatePlayerLogout(SocialPacketServerUpdatePlayerLogout& packet, std::shared_ptr<ClntServer> server) {
	m_socialPlayerManager.RemovePlayer(packet.m_gpid);
	//#DatabaseSave
}

void SocialServer::LoadPlayerSocialData(std::shared_ptr<SocialPlayer> pplayer) {
/*
*	Load from database
*	
*/

}
