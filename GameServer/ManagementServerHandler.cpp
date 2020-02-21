#include "ManagementServerHandler.h"
#include "ConnectionPacket.h"
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
	case transferChannelResponse: {
		GameToManagementTransferChannelResponse packet;
		packet.Deserialize(buffer);
		auto player = m_gameServer->GetPlayerManager().FindPlayer(packet.m_gpid);
		this->HandleTransferChannelResponse(packet.m_isSuccess, packet.m_channelName, player);
		break;
	}
	}
	
}

void ManagementServerHandler::HandleTrasferForInviteRequest(std::string ip, int port, std::shared_ptr<Player> player) {
	GameToManagementTransferChannelRequest packet;
	packet.m_ipAddress = ip;
	packet.m_portNum = port;
	packet.m_gpid = player->GetGPID();
	packet.m_session = player->GetSessionID();

	this->SendPacketToServer(packet);
}

void ManagementServerHandler::HandleTransferChannelResponse(bool success, std::string channel, std::shared_ptr<Player> pPlayer) {
	if (nullptr == pPlayer)
		return;

	ConnectionPacketTransferForInviteResponse packet;
	packet.m_channelName = channel;
	packet.m_isSuccess = success;

	pPlayer->SendPacket(packet);
}

void ManagementServerHandler::HandlePreLoadPacket(GPID gpid, SessionID session) {
	auto preClient = m_gameServer->GetPlayerManager().PreLoadClient(gpid, session);
	preClient->SetName(std::to_string(gpid));
	//#DatabaseLoad
	//preClient
}
