#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "LoginServer.h"
#include "LoginPacket.h"

LoginServer::LoginServer() {}
LoginServer::~LoginServer() {}

void LoginServer::HandleAcceptClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	playerManager.InsertPlayer(clientSocket);
	printf("Connect client[%d] Total players[%d]\n", clientSocket, playerManager.GetPlayerList().size());
}
void LoginServer::HandleDisconnectClient(SOCKET clientSocket) {
	if (clientSocket < 1)
		return;
	auto player = playerManager.FindPlayerBySocket(clientSocket);
	if (player == nullptr)
		return;
	playerManager.PlayerDisconnect(clientSocket);
	printf("Disconnect client[%d] Total players[%d]\n", clientSocket, playerManager.GetPlayerList().size());
}

void LoginServer::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo) return;

	Util::LoggingInfo("LoginServer.log", "Recv packet size: %d from %d", bufInfo->recvBufferSize, bufInfo->socket);

	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::basePacketTypeLogin: {
		this->HandlePacketLogin(bufInfo);
		break;
	}
	default: {
		Util::LoggingInfo("LoginServer.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
}

void LoginServer::HandlePacketLogin(BufferInfo* bufInfo) {
	LoginPacketType type = (LoginPacketType)PacketTypeDeserial(bufInfo->buffer);

	switch (type) {
	case clientLoginPacketTypeLoginRequest: {
		ClientLoginPacketTypeLoginRequest packetClientRequest{};
		packetClientRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((4 > packetClientRequest.userId.size()) || (packetClientRequest.userId.size() > ID_SIZE)) {
			flag = false;
		}
		if ((flag == true) && (8 > packetClientRequest.userPw.size()) || (packetClientRequest.userPw.size() > PW_SIZE)) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetClientRequest.userId.size(); ++i) {
				if (!isalnum(packetClientRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetClientRequest.userPw.size(); ++i) {
				if (!isalnum(packetClientRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}

		if (flag == true) {
			LoginManagementPacketTypeLoginRequest packetLoginRequest{};
			packetLoginRequest.userId = packetClientRequest.userId;
			packetLoginRequest.userPw = packetClientRequest.userPw;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginRequest.Serialize();
			send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
			LoginManagementPacketTypeLoginResponse packetLoginResponse{};
			packetLoginResponse.Deserialize(bufInfo->buffer);
			packetLoginResponse.serialNumber = 10; // serialNumber;

			ClientLoginPacketTypeLoginResponse packetClientResponse{};
			packetClientResponse.flag = packetLoginResponse.flag;
			packetClientResponse.userNick = packetLoginResponse.userNick;
			bufInfo->Clear();
			bufInfo->buffer = packetClientResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		else {
			ClientLoginPacketTypeLoginResponse packetClientResponse{};
			packetClientResponse.flag = flag;
			bufInfo->Clear();
			bufInfo->buffer = packetClientResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}
	case clientLoginPacketTypeLogoutRequest: {
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		LoginManagementPacketTypeLogoutRequest packetLoginRequest{};
		packetLoginRequest.serialNumber = 10; // serialnumber;
		bufInfo->Clear();
		bufInfo->buffer = packetLoginRequest.Serialize();
		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case clientLoginPacketTypeSignupRequest: {
		ClientLoginPacketTypeSignupRequest packetClientRequest{};
		packetClientRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		bool flag = true;
		if ((4 > packetClientRequest.userId.size()) || (packetClientRequest.userId.size() > ID_SIZE)) {
			flag = false;
		}
		if ((flag == true) && (8 > packetClientRequest.userPw.size()) || (packetClientRequest.userPw.size() > PW_SIZE)) {
			flag = false;
		}
		if ((flag == true) && (packetClientRequest.userNick.size() > NICK_SIZE)) {
			flag = false;
		}
		if (flag == true) {
			for (int i = 0; i < packetClientRequest.userId.size(); ++i) {
				if (!isalnum(packetClientRequest.userId[i])) {
					flag = false;
					break;
				}
			}
			for (int i = 0; i < packetClientRequest.userPw.size(); ++i) {
				if (!isalnum(packetClientRequest.userPw[i])) {
					flag = false;
					break;
				}
			}
		}
		if (flag == true) {
			LoginManagementPacketTypeSignupRequest packetLoginRequest{};
			packetLoginRequest.userId = packetClientRequest.userId;
			packetLoginRequest.userPw = packetClientRequest.userPw;
			packetLoginRequest.userNick = packetClientRequest.userNick;
			bufInfo->Clear();
			bufInfo->buffer = packetLoginRequest.Serialize();
			send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

			recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
			LoginManagementPacketTypeSignupResponse packetLoginResponse{};
			packetLoginResponse.Deserialize(bufInfo->buffer);

			ClientLoginPacketTypeSignupResponse packetClientResponse{};
			packetClientResponse.flag = packetLoginResponse.flag;
			bufInfo->Clear();
			bufInfo->buffer = packetClientResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		}
		else {
			ClientLoginPacketTypeSignupResponse packetClientResponse{};
			packetClientResponse.flag = flag;
			bufInfo->Clear();
			bufInfo->buffer = packetClientResponse.Serialize();
			send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
			Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);
		}
		break;
	}

	case clientLoginPacketTypeDeleteRequest: {
		ClientLoginPacketTypeDeleteRequest packetClientRequest{};
		packetClientRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeDeleteRequest packetLoginRequest{};
		packetLoginRequest.serialNumber = 10; // player serialNumber;
		bufInfo->Clear();
		bufInfo->buffer = packetLoginRequest.Serialize();
		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0); // 수정
		LoginManagementPacketTypeDeleteResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(bufInfo->buffer);

		ClientLoginPacketTypeDeleteResponse packetClientResponse{};
		// player serialNumber delete
		packetClientResponse.flag = packetLoginResponse.flag;
		bufInfo->Clear();
		bufInfo->buffer = packetClientResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case clientLoginPacketTypeShowChannelRequest: {
		ClientLoginPacketTypeShowChannelRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeShowChannelResponse packetLoginRequest{};
		bufInfo->Clear();
		bufInfo->buffer = packetLoginRequest.Serialize();
		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		LoginManagementPacketTypeShowChannelResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(bufInfo->buffer);

		ClientLoginPacketTypeShowChannelResponse packetClientResponse{};
		bufInfo->Clear();
		bufInfo->buffer = packetClientResponse.Serialize();
		send(bufInfo->socket, bufInfo->buffer, BUFFER_SIZE, 0);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Send packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		break;
	}

	case clientLoginPacketTypeChannelInRequest: {
		ClientLoginPacketTypeChannelInRequest packetRequest{};
		packetRequest.Deserialize(bufInfo->buffer);
		Util::LoggingInfo("LoginServer.log", "Type : %d%d || Recv packet : %s || size: %d || from %d", bufInfo->buffer[0], bufInfo->buffer[1], bufInfo->buffer, bufInfo->buffer.Length(), bufInfo->socket);

		LoginManagementPacketTypeChannelInResponse packetLoginRequest{};
		bufInfo->Clear();
		bufInfo->buffer = packetLoginRequest.Serialize();
		send(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);

		recv(managementServer, bufInfo->buffer, BUFFER_SIZE, 0);
		LoginManagementPacketTypeChannelInResponse packetLoginResponse{};
		packetLoginResponse.Deserialize(bufInfo->buffer);

		ClientLoginPacketTypeChannelInResponse packetClientResponse{};
		bufInfo->Clear();
		bufInfo->buffer = packetClientResponse.Serialize();
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
