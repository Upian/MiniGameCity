#include "SocialServer.h"
#include "BasePacket.h"
#include "SocialServerPacket.h"

SocialServer::SocialServer() {}
SocialServer::~SocialServer() {}

void SocialServer::InitializeSocialServer() {

}

void SocialServer::HandleAcceptClient(SOCKET gameServerSocket) {
	m_gameServers.InsertServer(gameServerSocket);
	
}

void SocialServer::HandleDisconnectClient(SOCKET clientSocket) {

}

void SocialServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	BasePacketType type = PacketTypeDeserial<BasePacketType>(bufInfo->buffer);


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
	PacketTypeSocialServer type = PacketTypeDeserial<PacketTypeSocialServer>(buffer);

	switch (type) {
	case PacketTypeSocialServer::updatePlayerInfo: {

		break;
	}
	default: {
		Util::LoggingInfo("SocialServer.log", "Recv wrong soical server packet ID: %d", type);
		break;
	}
	}
}