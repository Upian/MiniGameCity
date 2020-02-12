#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "ManagementServer.h"

ManagementServer::ManagementServer() {}
ManagementServer::~ManagementServer() {}

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
	case loginManagementPacketTypeLoginRequest: {
		LoginManagementPacketTypeLoginRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserIdMinSize") > packetLoginRequest.userId.size()) || (packetLoginRequest.userId.size() > Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserIdMaxSize"))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserPwMinSize") > packetLoginRequest.userPw.size()) || (packetLoginRequest.userPw.size() > Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserPwMaxSize"))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetLoginRequest.userId.size(); ++i) {
				if (!isalnum(packetLoginRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetLoginRequest.userPw.size(); ++i) {
				if (!isalnum(packetLoginRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			ManagementDBCachePacketTypeLoginRequest packetManagementRequest{};
			packetManagementRequest.userId = packetLoginRequest.userId;
			packetManagementRequest.userPw = packetLoginRequest.userPw;
			bufInfo->Clear();
			bufInfo->buffer = packetManagementRequest.Serialize();
			send(dbCache, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(dbCache, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
			ManagementDBCachePacketTypeLoginResponse packetManagementResponse{};
			packetManagementResponse.Deserialize(bufInfo->buffer);
			packetManagementResponse.GPID = 10; // GPID;

			LoginManagementPacketTypeLoginResponse packetLoginResponse{};
			packetLoginResponse.flag = packetManagementResponse.flag;
			packetLoginResponse.userNick = packetManagementResponse.userNick;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		else {
			LoginManagementPacketTypeLoginResponse packetLoginResponse{};
			packetLoginResponse.flag = flag;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}
	case loginManagementPacketTypeLogoutRequest: {
		LoginManagementPacketTypeLogoutRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		ManagementDBCachePacketTypeLogoutRequest packetManagementRequest{};
		bufInfo->Clear();
		bufInfo->buffer = packetManagementRequest.Serialize();
		send(dbCache, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginManagementPacketTypeSignupRequest: {
		LoginManagementPacketTypeSignupRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserIdMinSize") > packetLoginRequest.userId.size()) || (packetLoginRequest.userId.size() > Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserIdMaxSize"))) {
			flag = false;
		}
		if ((flag == true) && (Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserPwMinSize") > packetLoginRequest.userPw.size()) || (packetLoginRequest.userPw.size() > Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserPwMaxSize"))) {
			flag = false;
		}
		if ((flag == true) && (packetLoginRequest.userNick.size() > Util::GetConfigToInt("ManagementServer.ini", "Definition", "UserNickMaxSize"))) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetLoginRequest.userId.size(); ++i) {
				if (!isalnum(packetLoginRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetLoginRequest.userPw.size(); ++i) {
				if (!isalnum(packetLoginRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}
		if (flag == true) {
			ManagementDBCachePacketTypeSignupRequest packetManagementRequest{};
			packetManagementRequest.userId = packetLoginRequest.userId;
			packetManagementRequest.userPw = packetLoginRequest.userPw;
			packetManagementRequest.userNick = packetLoginRequest.userNick;
			bufInfo->Clear();
			bufInfo->buffer = packetManagementRequest.Serialize();
			send(dbCache, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(dbCache, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
			ManagementDBCachePacketTypeSignupResponse packetManagementResponse{};
			packetManagementResponse.Deserialize(bufInfo->buffer);

			LoginManagementPacketTypeSignupResponse packetLoginResponse{};
			packetLoginResponse.flag = packetManagementResponse.flag;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			LoginManagementPacketTypeSignupResponse packetLoginResponse{};
			packetLoginResponse.flag = flag;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}

	case loginManagementPacketTypeDeleteRequest: {
		LoginManagementPacketTypeDeleteRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		ManagementDBCachePacketTypeDeleteRequest packetManagementRequest{};
		packetManagementRequest.GPID = 10; // player GPID;
		bufInfo->Clear();
		bufInfo->buffer = packetManagementRequest.Serialize();
		send(dbCache, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(dbCache, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
		ManagementDBCachePacketTypeDeleteResponse packetManagementResponse{};
		packetManagementResponse.Deserialize(bufInfo->buffer);

		LoginManagementPacketTypeDeleteResponse packetLoginResponse{};
		// player GPID delete
		packetLoginResponse.flag = packetManagementResponse.flag;
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginManagementPacketTypeShowChannelRequest: {
		LoginManagementPacketTypeShowChannelRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeShowChannelResponse packetLoginResponse{};
		packetLoginResponse.channel[0] = this->channel[0];
		packetLoginResponse.channel[1] = this->channel[1];
		packetLoginResponse.channel[2] = this->channel[2];
		packetLoginResponse.channel[3] = this->channel[3];
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginManagementPacketTypeChannelInRequest: {
		LoginManagementPacketTypeChannelInRequest packetLoginRequest{};
		packetLoginRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeChannelInResponse packetLoginResponse{};
		for (int i = 0; i < Util::GetConfigToInt("Management.ini", "Definition", "ChannelSize"); ++i) {
			if ((packetLoginRequest.channelName == this->channel[i].channelName)
				&& (this->channel[i].numberOfPeople < this->channel[i].limitOfPeople)) {
				packetLoginResponse.flag = true;
				packetLoginResponse.ip = Util::GetConfigToString("Management.ini", "Network", "GameServerIP", "10.255.252.100");
				packetLoginResponse.port = Util::GetConfigToInt("Management.ini", "Network", "Port", 10000);
			}
			else {
				packetLoginResponse.flag = false;
			}
		}
		bufInfo->Clear();
		bufInfo->buffer = packetLoginResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("ManagementServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}
	default: {
		Util::LoggingInfo("ManagementServer.log", "Recv wrong login packet ID : %d", type);
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
	address.sin_port = htons(Util::GetConfigToInt("ManagementServer.ini", "Network", "DBCachePort", 20000));
	if (SOCKET_ERROR == connect(dbCache, (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("ManagementServer.log", "ERROR - Can not connect to db server");
		_exit(0);
	}

	Util::LoggingInfo("ManagementServer.log", "Success - connect to db server.");

	//send packet 	
}
