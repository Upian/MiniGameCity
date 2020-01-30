#include "Client.h"
#include "Log.h"
#include "BasePacket.h"
#include "ServerCommon.h"

// Should I put this function in BaseServer or here?
void Client::SendPacket(BasePacket& packet) {
	WSABUF dataBuf;
	dataBuf.buf = packet.Serialize();
	dataBuf.len = sizeof(dataBuf.buf);

	WSAEVENT wsaEvent = WSACreateEvent();

	WSAOVERLAPPED overlapped;
	ZeroMemory(&overlapped, sizeof(overlapped));
	overlapped.hEvent = wsaEvent;

	DWORD sendBytes = 0;

	if (SOCKET_ERROR == WSASend(m_socket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL)) {
		Util::LoggingError("Network.log", "Send error");
		return;
	}
	Util::LoggingError("Network.log", "Socket[%d], sendByte[%d]", m_socket, sendBytes);
}
