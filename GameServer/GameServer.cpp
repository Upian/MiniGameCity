#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <string>
#include <list>
#include <thread>
#include <ctime>


#define		SERVERPORT	50
#define		BUFSIZE 	512

void readFile(std::vector<std::string> &v, std::string text_name);
void err_quit(const char * msg);
void err_display(const char *msg);

int main()
{
	std::vector<std::string> words, sentens;
	//int room_number, game_type, message_type;
	std::list<int> player_list;
	int retval;


	readFile(words, "words.txt");
	readFile(sentens, "sentens.txt");

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1) {
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			break;
		}

		std::cout << "\n" << "[TCP 서버]클라이언트 접속 : IP 주소 = " << inet_ntoa(clientaddr.sin_addr) << " 포트 번호 = " << ntohs(clientaddr.sin_port) << std::endl;

		retval = recv(client_sock, buf, sizeof(buf), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0)
		{
			break;
		}

		std::cout << "클라이언트에서 데이터를 받았습니다 : " << buf << std::endl;

		srand((unsigned int)time(0));

		int choice_word = rand() % (words.size() - 1);
		strcpy_s(buf, words[choice_word].size(), words[choice_word].c_str());

		retval = send(client_sock, buf, sizeof(buf), 0);



	}


}

class gameBase {
private:
	int room_number, game_round;
	std::list<int> player_list;

};

void readFile(std::vector<std::string> &v, std::string text_name) {

	std::ifstream read_file;
	std::string temp_word;
	read_file.open(text_name);
	if (read_file.is_open()) {
		while (getline(read_file, temp_word)) {
			v.push_back(temp_word);
		}
	}
	read_file.close();
}

void err_quit(const char * msg) {

	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void err_display(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
