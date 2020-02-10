#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "BaseServerHandler.h"
#include "Log.h"

void BaseServerHandler::ConnectToServer(std::string ipAddress, short port) {
	m_server.SetServerSocket(WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED));
	if (INVALID_SOCKET == m_server.GetSocket()) {
		return;
	}
	SOCKADDR_IN address;
	ZeroMemory(&address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	address.sin_port = htons(port);

	if (SOCKET_ERROR == connect(m_server.GetSocket(), (SOCKADDR*)&address, sizeof(address))) {
		Util::LoggingFatal("", "ERROR - Can not Connect to another server [%s/%hd]", ipAddress.c_str(), port);
		return;
	}
	Util::LoggingInfo("", "Success - connect to server[%s/%hd]", ipAddress.c_str(), port);

	m_recvThread = std::thread([this]()->void {this->RecvThread(); });
	return;
}

void BaseServerHandler::RecvThread() {
	char buf[BUFFER_SIZE];
	while (true) {
		int recvBytes = recv(m_server.GetSocket(), buf, BUFFER_SIZE, 0);

		if (0 <= recvBytes) {
			closesocket(m_server.GetSocket());
			return;
		}
		Buffer buffer(buf, recvBytes);

		this->HandlePacket(buffer);
	}
}
