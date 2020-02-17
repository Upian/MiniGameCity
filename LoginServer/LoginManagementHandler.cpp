#include "LoginManagementHandler.h"
#include "LoginPacket.h"
#include "LoginServer.h"
#include "Player.h"
#include "Log.h"

void LoginManagementHandler::Initialize() {
	m_loginServer = LoginServer::GetServer();
}

/*

		  Client -> Login server

*/

void LoginManagementHandler::HandleLoginPacket(Buffer& buffer, std::shared_ptr<Player> player, LoginPacketType type) {
	if (player == nullptr)
		return;

	switch (type) {
	case LoginPacketType::clientLoginPacketTypeShowChannelRequest: {
		//Util::LoggingInfo("LoginServer.log", "Type : LoginManagementPacketTypeLoginResponse || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		ClientLoginPacketTypeShowChannelRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		this->HandlePacketShowChannelRequest(packetClientRequest, player);
		break;
	}
	case LoginPacketType::clientLoginPacketTypeChannelInRequest: {
		ClientLoginPacketTypeChannelInRequest packetClientRequest{};
		packetClientRequest.Deserialize(buffer);
		this->HandlePacketChannelInRequest(packetClientRequest, player);
		break;
	}
	default:
		break;
	}
}

/*

		  Login Server -> Management Server

*/

void LoginManagementHandler::HandlePacketShowChannelRequest(ClientLoginPacketTypeShowChannelRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginManagementPacketTypeShowChannelRequest packetLoginRequest{};
	this->SendPacketToServer(packetLoginRequest);
}
void LoginManagementHandler::HandlePacketChannelInRequest(ClientLoginPacketTypeChannelInRequest& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	LoginManagementPacketTypeChannelInRequest packetLoginRequest{};
	packetLoginRequest.channelName = packet.channelName;
	this->SendPacketToServer(packetLoginRequest);
}

/*

		  Management -> Login server

*/

void LoginManagementHandler::HandlePacket(Buffer& buffer) {
	BasePacketType type = (BasePacketType)PacketTypeDeserial(buffer);
	if (type != BasePacketType::loginPacketTypeManagement)
		return;

	ManagementPacketType managementType = (ManagementPacketType)PacketTypeDeserial(buffer);

	switch (managementType) {
	case ManagementPacketType::loginManagementPacketTypeShowChannelResponse: {
		LoginManagementPacketTypeShowChannelResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(buffer);
		auto player = m_loginServer->GetPlayerManager().FindPlayer(packetLoginResponse.gpid);
		this->HandlePacketShowChannelResponse(packetLoginResponse, player);
		break;
	}
	case ManagementPacketType::loginManagementPacketTypeChannelInResponse: {
		LoginManagementPacketTypeChannelInResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(buffer);
		auto player = m_loginServer->GetPlayerManager().FindPlayer(packetLoginResponse.gpid);
		this->HandlePacketChannelInResponse(packetLoginResponse, player);
		break;
	}
	default:
		break;
	}
}

/*

		  Login Server -> Client

*/

void LoginManagementHandler::HandlePacketShowChannelResponse(LoginManagementPacketTypeShowChannelResponse& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	// 채널 수정 예정
	ClientLoginPacketTypeShowChannelResponse packetClientResponse{};
	packetClientResponse.channel[0] = packet.channel[0];
	packetClientResponse.channel[1] = packet.channel[1];
	packetClientResponse.channel[2] = packet.channel[2];
	packetClientResponse.channel[3] = packet.channel[3];
	player->SendPacket(packetClientResponse);
}

void LoginManagementHandler::HandlePacketChannelInResponse(LoginManagementPacketTypeChannelInResponse& packet, std::shared_ptr<Player> player) {
	if (player == nullptr)
		return;

	ClientLoginPacketTypeChannelInResponse packetClientResponse{};
	packetClientResponse.flag = packet.flag;
	packetClientResponse.ip = packet.ip;
	packetClientResponse.port = packet.port;
	player->SendPacket(packetClientResponse);
}