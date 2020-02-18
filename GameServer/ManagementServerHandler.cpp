#include "ManagementServerHandler.h"
#include "GameServer.h"
#include "ManagementPacket.h"

void ManagementServerHandler::Initialize() {
	m_gameServer = GameServer::GetServer();
	this->ConnectToServer(
		Util::GetConfigToString("GameServer.ini", "Network", "ManagementServerIP", "127.0.0.1").c_str(),
		Util::GetConfigToInt("GameServer.ini", "Network", "ManagementServerPort", 19998)
	);

	this->RegisterToServer();
}

void ManagementServerHandler::RegisterToServer() {
	GameToManagementResistarServerInfo packet;

	this->SendPacketToServer(packet);
}

void ManagementServerHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);
	if (BasePacketType::basePacketTypeGameToManagementServer != type)
		return;


}
