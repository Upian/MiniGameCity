#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "LoginServer.h"
#include "LoginPacket.h"

LoginServer::LoginServer() {}
LoginServer::~LoginServer() {}

void LoginServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo) return;

	Util::LoggingInfo("LoginServer.log", "Recv packet size: %d from %d", bufInfo->recvBufferSize, bufInfo->socket);

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::clientBasePacketTypeLogin: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("LoginServer.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
}

void LoginServer::HandleAcceptClient(SOCKET clientSocket) {

}
void LoginServer::HandleDisconnectClient(SOCKET clientScoket) {

}

void LoginServer::HandlePacketLogin(BufferInfo* bufInfo) {
	LoginPacketType type = (LoginPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case loginPacketTypeLoginRequest: {
		LoginPacketTypeLoginRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginPacketTypeLoginResponse packetResponse{};

		if ((4 > packetRequest.accountId.size()) || (packetRequest.accountId.size() > ID_SIZE)) {
			packetResponse.flag = false;
		}
		if ((packetResponse.flag == true) && (8 > packetRequest.accountPw.size()) || (packetRequest.accountPw.size() > PW_SIZE)) {
			packetResponse.flag = false;
		}
		if (packetResponse.flag == true) {
			for (int i = 0; i < packetRequest.accountId.size(); ++i) {
				if (!isalnum(packetRequest.accountId[i])) {
					packetResponse.flag = false;
					break;
				}
			}
			for (int i = 0; i < packetRequest.accountPw.size(); ++i) {
				if (!isalnum(packetRequest.accountPw[i])) {
					packetResponse.flag = false;
					break;
				}
			}
		}

		if (packetResponse.flag == true) {
			send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		else {
			char* buf = packetResponse.Serialize();
			send(bufInfo->socket, buf, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}
	case loginPacketTypeLogoutRequest: {
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginPacketTypeSignupRequest: {
		LoginPacketTypeSignupRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginPacketTypeSignupResponse packetResponse{};

		if ((4 > packetRequest.accountId.size()) || (packetRequest.accountId.size() > ID_SIZE)) {
			packetResponse.flag = false;
		}
		if ((packetResponse.flag == true) && (8 > packetRequest.accountPw.size()) || (packetRequest.accountPw.size() > PW_SIZE)) {
			packetResponse.flag = false;
		}
		if ((packetResponse.flag == true) && (packetRequest.nick.size() > NICK_SIZE)) {
			packetResponse.flag = false;
		}
		if (packetResponse.flag == true) {
			for (int i = 0; i < packetRequest.accountId.size(); ++i) {
				if (!isalnum(packetRequest.accountId[i])) {
					packetResponse.flag = false;
					break;
				}
			}
			for (int i = 0; i < packetRequest.accountPw.size(); ++i) {
				if (!isalnum(packetRequest.accountPw[i])) {
					packetResponse.flag = false;
					break;
				}
			}
		}
		if (packetResponse.flag == true) {
			send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			char* buf = packetResponse.Serialize();
			send(bufInfo->socket, buf, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}

	case loginPacketTypeDeleteRequest: {
		LoginPacketTypeDeleteRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginPacketTypeDeleteResponse packetResponse{};

		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginPacketTypeShowChannelRequest: {
		LoginPacketTypeShowChannelRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginPacketTypeShowChannelResponse packetResponse{};

		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case loginPacketTypeChannelInRequest: {
		LoginPacketTypeChannelInRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginPacketTypeChannelInResponse packetResponse{};

		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}
	default: {
		Util::LoggingInfo("LoginServer.log", "Recv wrong login packet ID : %d", type);
	}
	}
}

void LoginServer::InitializeLoginServer() {
	this->ConnectToManagementServer();

}

void LoginServer::ConnectToManagementServer() {
	managementServer = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == managementServer) {
		Util::LoggingError("LoginServer.log", "ERROR - Can not create socket");
		return;
	}

	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(Util::GetConfigToString("LoginServer.ini", "Network", "ManagementServerIP", "127.0.0.1").c_str()); // 10.255.252.95
	address.sin_port = htons(Util::GetConfigToInt("LoginServer.ini", "Network", "ManagementServerPort", 19998)); // 19999

	if (SOCKET_ERROR == connect(managementServer, (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("LoginServer.log", "ERROR - Can not connect to management server");
		_exit(0);
	}

	Util::LoggingInfo("LogingServer.log", "Success - connect to management server.");

	//send packet 	
}
