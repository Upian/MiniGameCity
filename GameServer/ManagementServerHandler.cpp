#include "ManagementServerHandler.h"
#include "GameServer.h"
#include "ManagementPacket.h"

void ManagementServerHandler::Initialize() {
	m_gameServer = GameServer::GetServer();
	this->ConnectToServer(
		Util::GetConfigToString("GameServer.ini", "Network", "ManagementServerIP", "127.0.0.1").c_str(),
		Util::GetConfigToInt("GameServer.ini", "Network", "ManagementServerPort", 19999)
	);

	this->RegisterToServer();
}

void ManagementServerHandler::RegisterToServer() {
	GameToManagementRegisterServerInfo packet;
	
	// ip port Ãß°¡
	packet.m_ipAddress = m_gameServer->GetServerIPAddress();
	packet.m_portNum = m_gameServer->GetServerPortNum();

	this->SendPacketToServer(packet);
}

void ManagementServerHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);

	if (BasePacketType::basePacketTypeGameToManagementServer != type)
		return;

	ManagementPacketType managementType = (ManagementPacketType)PacketTypeDeserial(buffer);
	switch (managementType) {
	case ManagementPacketType::preLoadClient: {
		GameToManagementPreLoadRequest packet;
		packet.Deserialize(buffer);
		this->HandlePreLoadPacket(packet.m_gpid, packet.m_token);
		break;
	}
	}
	
}
namespace {
	int testGpid = 0;
	char testName = 'a';
}
void ManagementServerHandler::HandlePreLoadPacket(GPID gpid, SessionID session) {
	auto preClient = m_gameServer->GetPlayerManager().PreLoadClient(gpid, session);
	std::string name = "Client ";
	preClient->SetName(name + testName);
	preClient->SetGPID(testGpid);
	//#DatabaseLoad
	//preClient
}
