#include "ClientBase.h"
#include "Log.h"
#include "BasePacket.h"
#include "ServerCommon.h"

// Should I put this function in BaseServer or here?


bool ClientBase::SendPacket(BasePacket& packet) {
	m_clientBuffer.Clear();

	m_clientBuffer.buffer = packet.Serialize();
	m_clientBuffer.dataBuf.len = m_clientBuffer.buffer.Length();
	m_clientBuffer.overlapped.hEvent = WSACreateEvent();
	DWORD sendBytes = 0;
	if (SOCKET_ERROR == WSASend(m_socket, 
		&m_clientBuffer.dataBuf, 
		1, 
		&sendBytes, 
		0, 
		NULL,//&m_clientBuffer.overlapped, 
		NULL)) {
		if(WSA_IO_PENDING != WSAGetLastError())
		Util::LoggingError("Network.log", "Send error[%d]", WSAGetLastError());
		return false;
	}
	return true;
}

bool ClientBase::SendPacket(Buffer & buffer) {
	m_clientBuffer.Clear();

	m_clientBuffer.buffer = buffer;
	m_clientBuffer.dataBuf.len = buffer.Length();
	m_clientBuffer.overlapped.hEvent = WSACreateEvent();
	DWORD sendBytes = 0;

	if (SOCKET_ERROR == WSASend(
		m_socket,
		&m_clientBuffer.dataBuf,
		1,
		&sendBytes,
		0,
		NULL,
		NULL)) {
		Util::LoggingError("Network.log", "Send error[%d]", WSAGetLastError());
		return false;
	}
	return true;
}
