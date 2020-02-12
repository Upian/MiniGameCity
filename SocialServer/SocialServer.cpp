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
	case BasePacketType::basePacketTypeSocialServer: {
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
	case PacketTypeSocialServer::acceptPlayerLogin: { 
		SocialPacketServerAcceptPlayerLogin packet;
		packet.Deserialize(bufInfo->buffer);
		this->HandleAcceptPlayerLogin(packet, m_gameServers.FindServerBySocket(bufInfo->socket));
		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong soical server packet ID: %d", type);
		break;
	}
	}
}

void SocialServer::HandleAcceptPlayerLogin(SocialPacketServerAcceptPlayerLogin& packet, std::shared_ptr<ClntServer> server) {
	if (nullptr == server) 
		return;
	
	std::shared_ptr<SocialPlayer> pplayer = m_socialPlayerManager.InsertPlayer(packet.m_gpid, server);
	if (nullptr == pplayer)
		return;

	pplayer->SetName(packet.m_name); //#Test
	Util::LoggingDebug("", "---------------------------------------------");
	for (auto k : m_socialPlayerManager.GetSocialPlayerList()) {
		Util::LoggingDebug("", "%s[%u]", k->GetName().c_str(), k->GetGPID());
	}
	Util::LoggingDebug("", "---------------------------------------------");
}

void SocialServer::LoadPlayerSocialData(std::shared_ptr<SocialPlayer> pplayer) {
/*
*	Load from database
*	
*/

}
