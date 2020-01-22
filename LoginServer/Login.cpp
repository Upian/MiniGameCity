#include "Login.h"

LoginServer::LoginServer() {}
LoginServer::~LoginServer() {}


/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void LoginServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf

	//switch(packet type) ...

	printf("Login server -- recv packet\n");

}

void LoginServer::HandleAcceptClient(SOCKET clientSocket) {

}