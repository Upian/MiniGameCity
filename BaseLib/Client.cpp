#include "Client.h"
#include "Log.h"
#include "BasePacket.h"
#include "ServerCommon.h"

// Should I put this function in BaseServer or here?


bool Client::SendPacket(BasePacket& packet) {
	m_clientBuffer.Clear();
	m_clientBuffer.dataBuf.buf = packet.Serialize();
	m_clientBuffer.dataBuf.len = sizeof(m_clientBuffer.dataBuf.len);
	m_clientBuffer.overlapped.hEvent = WSACreateEvent();
	DWORD sendBytes = 0;

	if (SOCKET_ERROR == WSASend(m_socket, 
		&m_clientBuffer.dataBuf, 
		1, 
		&sendBytes, 
		0, 
		&m_clientBuffer.overlapped, 
		NULL)) {
		Util::LoggingError("Network.log", "Send error");
		return false;
	}
	return true;
}
