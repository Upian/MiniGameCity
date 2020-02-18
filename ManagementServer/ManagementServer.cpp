#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ManagementServer.h"

ManagementServer::ManagementServer() {}
ManagementServer::~ManagementServer() {}

// Game Server와 통신 필요함.
// ShowChannel 누르면 게임서버로부터 인원들 파악해서 가져와야 함.

void ManagementServer::HandleAcceptClient(SOCKET clientSocket) {
	// 테스트용 하드코딩
	channel[0].channelName = "channel1";
	channel[0].numberOfPeople = 200;
	channel[0].limitOfPeople = 300;
	channel[1].channelName = "channel2";
	channel[1].numberOfPeople = 300;
	channel[1].limitOfPeople = 300;
	channel[2].channelName = "channel3";
	channel[2].numberOfPeople = 100;
	channel[2].limitOfPeople = 300;
	channel[3].channelName = "channel4";
	channel[3].numberOfPeople = 50;
	channel[3].limitOfPeople = 300;

}
void ManagementServer::HandleDisconnectClient(SOCKET clientSocket) {

}

/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void ManagementServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf
	if (nullptr == bufInfo) return;

	Util::LoggingInfo("ManagementServer.log", "Recv packet size: %d", bufInfo->recvBufferSize);
	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::loginPacketTypeManagement: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	case BasePacketType::basePacketTypeGameToManagementServer: {

		break;
	}
													// 	case basePacketTypeRoom: {
													// 		break;
													// 	}
													// 	case basePacketTypeGame: {
													// 		break;
													// 	}
													// 	case basePacketTypeShop: {
													// 		break;
													// 	}
													// 	case basePacketTypeRanking: {
													// 		break;
													// 	}
													// 	case basePacketTypeSocial: {
													// 		break;
													// 	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
	printf("Management server -- recv packet: %s\n", bufInfo->dataBuf.buf);

}

void ManagementServer::HandlePacketLogin(BufferInfo* bufInfo) {
	ManagementPacketType type = (ManagementPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case loginManagementPacketTypeShowChannelRequest: {
		LoginManagementPacketTypeShowChannelRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : LoginManagementPacketTypeShowChannelRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeShowChannelResponse packetLoginResponse{};
		packetLoginResponse.channel[0] = this->channel[0];
		packetLoginResponse.channel[1] = this->channel[1];
		packetLoginResponse.channel[2] = this->channel[2];
		packetLoginResponse.channel[3] = this->channel[3];
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : LoginManagementPacketTypeShowChannelResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginManagementPacketTypeChannelInRequest: {
		LoginManagementPacketTypeChannelInRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : LoginManagementPacketTypeChannelInRequest || Recv packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		// 이후 port 번호 수정할 예정.
		LoginManagementPacketTypeChannelInResponse packetLoginResponse{};
		for (int i = 0; i < Util::GetConfigToInt("Management.ini", "Definition", "ChannelSize"); ++i) {
			if ((packetLoginRequest.channelName == this->channel[i].channelName)
				&& (this->channel[i].numberOfPeople < this->channel[i].limitOfPeople)) {
				packetLoginResponse.flag = true;
				packetLoginResponse.ip = Util::GetConfigToString("Management.ini", "Network", "GameServerIP", "127.0.0.1");
				packetLoginResponse.port = Util::GetConfigToInt("Management.ini", "Network", "Port", 10010);
			}
			else {
				packetLoginResponse.flag = false;
			}
		}
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : LoginManagementPacketTypeChannelInResponse || Send packet : %s || size: %d || from %d", bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong login packet ID : %d", type);
	}
	}
}

void ManagementServer::HandlePacketToGameServer(BufferInfo* bufInfo) {
	ManagementPacketType type = (ManagementPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case ManagementPacketType::registerServerInfo: {

		break;
	}
	case ManagementPacketType::updateServerInfo: {

		break;
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Handle game server packet, packet number: %d", type);
		break;
	}

	}

}

void ManagementServer::InitializeManagmentServer() {
	this->ConnectToDBCache();

}

void ManagementServer::ConnectToDBCache() {
	dbCache = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == dbCache) {
		Util::LoggingError("ManagementServer.log", "ERROR - Can not create socket");
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(Util::GetConfigToString("ManagementServer.ini", "Network", "DBCacheIP", "127.0.0.1").c_str());
	address.sin_port = htons(Util::GetConfigToInt("ManagementServer.ini", "Network", "DBCachePort", 30000));
	if (SOCKET_ERROR == connect(dbCache, (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("ManagementServer.log", "ERROR - Can not connect to db server");
		return;
	}

	Util::LoggingInfo("ManagementServer.log", "Success - connect to db server.");

	//send packet 	
}
