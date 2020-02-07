#pragma once
#ifndef __BASELIB_BASE_SERVER_H__
#define __BASELIB_BASE_SERVER_H__

#include <thread>
#include <list>

#include "ServerCommon.h"
#include "PlayerManager.h"
#include "Log.h"
#include "Config.h"

/*
*	BaseServer must inherit as public
*	Declare MAKE_SERVER(class name) in 'private:' section
	Define constructor destructor
*	Define virtual void HandleBasePacket() function
*	This is Singleton class
*	--usage--
	class Server: public BaseServer<Server>
	Server::CreateServer();
	Server::GetServer()->SetPortNum(port number)
	Server::GetServer()->SetServerName(Server name)
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
	Util::ConfigManager* GetConfigManager() const { return m_configManager; }

	void SetPortNum(int portNum) { m_portNum = portNum; }
	void SetServerName(std::string str = "") { m_serverName = str; }
	void InitializeBaseServer();
	void RunServer();
	
protected:
	BaseServer();
	virtual ~BaseServer();

private:
	virtual void HandleAcceptClient(SOCKET clientSocket) = 0;
	virtual void HandleDisconnectClient(SOCKET clientSocket) = 0;
	virtual void HandleBasePacket(BufferInfo* bufInfo) = 0;

	static T_Server* m_instance;
	Util::ConfigManager* m_configManager;

	void CreateIOWorkerThread();
	void IOWorkerThread();

	int m_portNum = 0;
	std::string m_serverName = "BaseServer";

	WSADATA m_wsaData;
	SOCKET m_serverSocket = 0; //
	
	HANDLE m_iocpHandle = nullptr;
	bool m_runningThread = true;
	std::list<std::thread*> m_iocpList;
	//다른 서버 리스트
	
	
};

#pragma region Singleton
template <typename T_Server>
T_Server* BaseServer<T_Server>::m_instance = nullptr;

/*
*	Create server
*/
template <typename T_Server>
T_Server* BaseServer<T_Server>::CreateServer() {
	if (nullptr == m_instance)
		m_instance = new T_Server();

	return m_instance;
}

template <typename T_Server>
template <typename ...T_Args>
T_Server* BaseServer<T_Server>::CreateServer(const T_Args& ...args) {
	if (nullptr == m_instance)
		m_instance = new T_Server(args...);

	return m_instance;
}

/*
*	Get server
*/
template <typename T_Server>
T_Server* BaseServer<T_Server>::GetServer() {
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
		friend class BaseServer<ClassName>; ClassName(); ~ClassName();\
		virtual void HandleAcceptClient(SOCKET clientSocket) override;\
		virtual void HandleDisconnectClient(SOCKET clientSocket) override;\
		virtual void HandleBasePacket(BufferInfo* bufInfo) override;
		

#endif // !MAKE_SERVER

#pragma endregion Make server to singleton

template<typename T_Server>
BaseServer<T_Server>::BaseServer() {
	Util::LogManager::CreateInstance();
	m_configManager = Util::ConfigManager::CreateInstance();
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
		Util::LoggingError(m_serverName + ".log", "WSAStartup error!");
		_exit(0);
		return;
	}

	m_serverSocket = WSASocketW(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == m_serverSocket) {
		Util::LoggingError(m_serverName + ".log", "Make socket error! socket: %d", m_serverSocket);
		_exit(0);
		return;
	}
	Util::LoggingInfo(m_serverName + ".log", "Make socket success! socket: %d, , Port: %d", m_serverSocket, m_portNum);

	SOCKADDR_IN serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	if (0 == m_portNum) {
		Util::LoggingError(m_serverName + ".log", "Error port number: 0");
		_exit(0);
		return;
	}
	serverAddress.sin_port = htons(m_portNum);

	if (SOCKET_ERROR == bind(m_serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress))) {
		Util::LoggingError(m_serverName + ".log", "Bind socket error!");
		closesocket(m_serverSocket);
		WSACleanup();
		_exit(0);
		return;
	}
	Util::LoggingInfo(m_serverName + ".log", "Bind socket success!");
	if (SOCKET_ERROR == listen(m_serverSocket, 5)) {
		Util::LoggingError(m_serverName + ".log", "Listen error!");
		closesocket(m_serverSocket);
		WSACleanup();
		_exit(0);
	}
	Util::LoggingInfo(m_serverName + ".log", "Listen success!");
#pragma endregion make socket and bind

#pragma region IOCP
	m_iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (nullptr == m_iocpHandle) {
		Util::LoggingError(m_serverName + ".log", "IOCP Handle error!");
		_exit(0);
	}

	this->CreateIOWorkerThread();
#pragma endregion Create IOCP handle

	Util::LoggingInfo(m_serverName + ".log", "---------------------------------------------------------------");
	Util::LoggingInfo(m_serverName + ".log", "--------------------- %s Start -----------------------", m_serverName.c_str());
	Util::LoggingInfo(m_serverName + ".log", "---------------------------------------------------------------");

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
			Util::LoggingError(m_serverName + ".log", "Accept client error!");
			continue;
		}

		m_iocpHandle = CreateIoCompletionPort((HANDLE)clientSocket, m_iocpHandle, clientSocket, 0);

		tempBuffer = new BufferInfo;
		if (nullptr == tempBuffer) {
			Util::LoggingError(m_serverName + ".log", "Can not create BufferInfo");
			continue;
		}
		ZeroMemory(&tempBuffer->overlapped, sizeof(tempBuffer->overlapped));
		tempBuffer->socket = clientSocket;
		tempBuffer->dataBuf.len = BUFFER_SIZE;
		tempBuffer->dataBuf.buf = tempBuffer->buffer;

		this->HandleAcceptClient(clientSocket);
		flag = 0;
		if (SOCKET_ERROR == WSARecv(tempBuffer->socket,
									&tempBuffer->dataBuf,
									1,
									&recvBytes,
									&flag,
									&tempBuffer->overlapped,
									NULL)
			&& WSA_IO_PENDING != WSAGetLastError()) {
			Util::LoggingError(m_serverName + ".log", "IO pendding error! code: %d", WSAGetLastError());
			continue;
		}
	
		Util::LoggingInfo(m_serverName + ".log", "Connect client socket[%d]", tempBuffer->socket);
	}
}

template<typename T_Server>
void BaseServer<T_Server>::CreateIOWorkerThread() {
	size_t cpuNum = std::thread::hardware_concurrency();

	Util::LoggingInfo(m_serverName + ".log", "Create threads - cpu cores: %d", 2*cpuNum);

	for (int i = 0; i < 2*cpuNum; ++i) {
		std::thread* th = new std::thread([this, i]()->void {
			this->IOWorkerThread();
		});

		m_iocpList.push_back(th);
	}
}

template<typename T_Server>
void BaseServer<T_Server>::IOWorkerThread() {
	BufferInfo* bufferInfo = nullptr;
	while (true == m_runningThread) {
		DWORD recvBytes = 0;
		SOCKET clientSocket;
		
		if (false == GetQueuedCompletionStatus(m_iocpHandle, &recvBytes, (PULONG_PTR)&clientSocket, (LPOVERLAPPED*)&bufferInfo, INFINITE) ||
			0 == recvBytes) {
			this->HandleDisconnectClient(clientSocket);

			Util::LoggingInfo(m_serverName + ".log", "socket[%d] is disconnected", clientSocket);
			if (nullptr != bufferInfo) {
				delete bufferInfo;
				bufferInfo = nullptr;
			}
			closesocket(clientSocket);
			continue;	
		}

		bufferInfo->socket = clientSocket;
		bufferInfo->dataBuf.len = recvBytes;
		bufferInfo->buffer.SetLength(recvBytes);

		this->HandleBasePacket(bufferInfo); //Handle packet
		//Comment out because of thread safe issues
		//Util::LoggingInfo("ServerMessage.log", "socket[%d] recv message: [%d]%s", clientSocket, bufferInfo->dataBuf.len, bufferInfo->dataBuf.buf);

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
			Util::LoggingError(m_serverName + ".log", "IO pendding error! code: %d", WSAGetLastError());
		}		
	}
}

#endif // !__BASELIB_BASE_SERVER_H__
