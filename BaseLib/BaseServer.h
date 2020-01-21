#pragma once
#ifndef __BASELIB_BASE_SERVER_H__
#define __BASELIB_BASE_SERVER_H__

#include "ServerCommon.h"
#include "PlayerManager.h"
#include "Log.h"
#include "Config.h"
#include <thread>

/*
*	BaseServer must inherit as public
*	This is Singleton class
*	--usage--
	class Server: public BaseServer<Server>
	Server::CreateServer();
	Server::GetServer()->InitializeBaseServer();
	Server::GetServer()->RunServer();
*/

template<typename T_Server>
class BaseServer {
public:
	static T_Server* CreateServer();
	template<typename ...T_Args> static T_Server* CreateServer(const T_Args& ...args);
	static void DestroyServer();
	static T_Server* GetServer();

	void InitializeBaseServer();
	void RunServer();
		
protected:
	BaseServer();
	virtual ~BaseServer();

private:
	static T_Server* m_instance;

	void CreateIOWorkerThread();
	void IOWorkerThread();

	int m_portNum = 0;
	
	WSADATA m_wsaData;
	SOCKET m_serverSocket  = 0;

	HANDLE m_iocpHandle = nullptr;
	bool m_runningThread = true;
	//다른 서버 리스트

};

#pragma region Singleton
template <typename T_Server>
typename T_Server* BaseServer<T_Server>::m_instance = nullptr;

/*
*	Create server
*/
template <typename T_Server>
typename T_Server* BaseServer<T_Server>::CreateServer() {
	if (nullptr == m_instance)
		m_instance = new T_Server();

	return m_instance;
}

template <typename T_Server>
template <typename ...T_Args>
typename T_Server* BaseServer<T_Server>::CreateServer(const T_Args& ...args) {
	if (nullptr == m_instance)
		m_instance == new T_Server(args...);

	return m_instance;
}

/*
*	Get server
*/
template <typename T_Server>
typename T_Server* BaseServer<T_Server>::GetServer() {
	return m_instance;
}

/*
*	Destroy server
*/
template <typename T_Server>
void BaseServer<T_Server>::DestroyServer() {
	delete m_instance;
	m_instance = nullptr;
}

#ifndef MAKE_SERVER
#define MAKE_SERVER( ClassName )\
		friend class BaseServer<ClassName>; ClassName(); ~ClassName();

#endif // !MAKE_SERVER

#pragma endregion Make server to singleton

template<typename T_Server>
BaseServer<T_Server>::BaseServer() :
	m_portNum(Util::GetConfigToInt("BaseServer.ini", "Network", "Port", 19998)) {
	Util::LogManager::CreateSingleton(
		Util::GetConfigToString("BaseServer.ini", "Definition", "LogDir", "../../../Logs/").c_str(),
		Util::GetConfigToBoolean("BaseServer.ini", "Definition", "ActiveConsoleLog", true),
		Util::GetConfigToBoolean("BaseServer.ini", "Definition", "ActiveLocalLog", true));
}

template<typename T_Server>
BaseServer<T_Server>::~BaseServer() {
	m_runningThread = false;
	WSACleanup();
}

template<typename T_Server>
void BaseServer<T_Server>::InitializeBaseServer() {
#pragma region Server socket
	if (0 != WSAStartup(MAKEWORD(2, 2), &m_wsaData)) {
		Util::Logging("BaseServer.log", "WSAStartup error!");
		_exit(0);
		return;
	}

	m_serverSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_serverSocket) {
		Util::Logging("BaseServer.log", "Make socket error! socket: %d", m_serverSocket);
		_exit(0);
		return;
	}
	Util::Logging("BaseServer.log", "Make socket success! socket: %d", m_serverSocket);

	SOCKADDR_IN serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(m_portNum);

	if (SOCKET_ERROR == bind(m_serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress))) {
		Util::Logging("BaseServer.log", "Bind socket error!");
		closesocket(m_serverSocket);
		WSACleanup();
		_exit(0);
		return;
	}
	Util::Logging("BaseServer.log", "Bind socket success!");
	if (SOCKET_ERROR == listen(m_serverSocket, 5)) {
		Util::Logging("BaseServer.log", "Listen error!");
		closesocket(m_serverSocket);
		WSACleanup();
		_exit(0);
	}
	Util::Logging("BaseServer.log", "Listen success!");
#pragma endregion make socket and bind

#pragma region IOCP
	m_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (nullptr == m_iocpHandle) {
		Util::Logging("BaseServer.log", "IOCP Handle error!");
		_exit(0);
	}

	this->CreateIOWorkerThread();
#pragma endregion 

	Util::Logging("BaseServer.log", "---------------------------------------------------------------");
	Util::Logging("BaseServer.log", "-------------------------Start server--------------------------");
	Util::Logging("BaseServer.log", "---------------------------------------------------------------");
	return;
}

template<typename T_Server>
void BaseServer<T_Server>::RunServer() {
	//player 객체

	SOCKET			clientSocket = 0;
	SOCKADDR_IN		clientAddress;
	int				AddressSize = sizeof(SOCKADDR_IN);
	DWORD			recvBytes;
	DWORD			flag;
	BufferInfo* tempBuffer = nullptr;
	while (true) {
		clientSocket = WSAAccept(m_serverSocket, (SOCKADDR*)&clientAddress, &AddressSize, NULL, NULL);
		if (INVALID_SOCKET == clientSocket) {
			Util::Logging("BaseServer.log", "Accept client error!");
			continue;
		}

		m_iocpHandle = CreateIoCompletionPort((HANDLE)clientSocket, m_iocpHandle, clientSocket, 0);

		tempBuffer = new BufferInfo;
		if (nullptr == tempBuffer) {
			Util::Logging("BaseServer.log", "Can not create BufferInfo");
			continue;
		}
		ZeroMemory(&tempBuffer->overlapped, sizeof(tempBuffer->overlapped));
		tempBuffer->socket = clientSocket;
		tempBuffer->dataBuf.len = BUFFER_SIZE;
		tempBuffer->dataBuf.buf = tempBuffer->buffer;
		
		flag = 0;
		if (SOCKET_ERROR == WSARecv(tempBuffer->socket,
									&tempBuffer->dataBuf,
									1,
									&recvBytes,
									&flag,
									&tempBuffer->overlapped,
									NULL)
			&& WSA_IO_PENDING != WSAGetLastError()) {
			Util::Logging("BaseServer.log", "IO pendding error! code: %d", WSAGetLastError());
			continue;
		}
	
		Util::Logging("BaseServer.log", "Connect client socket[%d]", tempBuffer->socket);
	}
}

template<typename T_Server>
void BaseServer<T_Server>::CreateIOWorkerThread() {
	size_t cpuNum = std::thread::hardware_concurrency();
	Util::Logging("BaseServer.log", "Create threads - cpu cores: %d", 2*cpuNum);

	for (int i = 0; i < 2*cpuNum; ++i) {
		std::thread* th = new std::thread([this, i]()->void {
			this->IOWorkerThread();
		});
	}
}

template<typename T_Server>
void BaseServer<T_Server>::IOWorkerThread() {
	BufferInfo* bufferInfo = nullptr;
	Util::Logging("BaseServer.log", "Start Thread");
	while (true == m_runningThread) {
		DWORD recvBytes = 0;
		SOCKET clientSocket;
		
		if (false == GetQueuedCompletionStatus(m_iocpHandle, &recvBytes, (PULONG_PTR)&clientSocket, (LPOVERLAPPED*)&bufferInfo, INFINITE)) {
			if(0 == recvBytes) {
				Util::Logging("BaseServer.log", "socket[%d] is disconnected", clientSocket);
				delete bufferInfo;
				bufferInfo = nullptr;
				closesocket(clientSocket);
			}
			continue;
		}

		bufferInfo->dataBuf.len = recvBytes;

		//Comment out because of thread safe issues
		//Util::Logging("ServerMessage.log", "socket[%d] recv message: [%d]%s", clientSocket, bufferInfo->dataBuf.len, bufferInfo->dataBuf.buf);
		
		bufferInfo->Clear();
		DWORD flag = 0;
		if (SOCKET_ERROR == WSARecv(clientSocket,
			&(bufferInfo->dataBuf),
			1,
			&recvBytes,
			&flag,
			&(bufferInfo->overlapped),
			NULL)
			&& WSA_IO_PENDING != WSAGetLastError()) {
			Util::Logging("BaseServer.log", "IO pendding error! code: %d", WSAGetLastError());
		}		
	}
}


#endif // !__BASELIB_BASE_SERVER_H__
