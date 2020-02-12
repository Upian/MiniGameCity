#include "SocialServer.h"
#include "BasePacket.h"
#include "SocialServerPacket.h"

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

//	BasePacketType type = PacketTypeDeserial<BasePacketType>(bufInfo->buffer);
	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case BasePacketType::basePacketTypeSocialServer: {
		this->HandleBaseSocialPacket(bufInfo->buffer);
		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong base packet ID: %d", type);
		break;
	}
		
	}
}

void SocialServer::HandleBaseSocialPacket(Buffer& buffer) {
	PacketTypeSocialServer type = (PacketTypeSocialServer)PacketTypeDeserial(buffer);

	switch (type) {
	case PacketTypeSocialServer::updatePlayerInfo: { 
		SocialPacketServerUpdatePlayerInfo packet;
		packet.Deserialize(buffer);
		this->LoadPlayerSocialData(packet.m_gpid);
		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong soical server packet ID: %d", type);
		break;
	}
	}
}


void SocialServer::LoadPlayerSocialData(GPID gpid) {
	printf("accept %d", gpid);
/*
*	Load from database
*	
*/
}
