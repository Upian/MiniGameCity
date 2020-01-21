#pragma once
#ifndef __BASELIB_SERVER_COMMON_H__
#define __BASELIB_SERVER_COMMON_H__

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 512

enum ServerType{
	serverTypeNone = 0,

	serverTypeLogin,
	serverTypeLobby,
	serverTypeGame,
	serverTypeDBManager,
	serverTypeSocial,

	serverTypeCount,
};


struct BufferInfo {
	void Clear() {
		ZeroMemory(&(overlapped), sizeof(WSAOVERLAPPED));
		dataBuf.len = BUFFER_SIZE;
		dataBuf.buf = buffer;
		ZeroMemory(buffer, BUFFER_SIZE);
		recvBufferSize = 0;
		sendBufferSize = 0;
	}

	WSAOVERLAPPED	overlapped;
	WSABUF			dataBuf;
	SOCKET			socket;
	char			buffer[BUFFER_SIZE] = {'\0', };
	size_t			recvBufferSize = 0;
	size_t			sendBufferSize = 0; 
};

#endif //!__BASELIB_SERVER_COMMON_H__