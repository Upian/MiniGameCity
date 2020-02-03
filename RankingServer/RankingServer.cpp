#include "RankingServer.h"

RankingServer::RankingServer() {}
RankingServer::~RankingServer() {}



void RankingServer::HandleAcceptClient(SOCKET clientSocket) {

}
void RankingServer::HandleDisconnectClient(SOCKET clientSocket) {

}
/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void RankingServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf

	//switch(packet type) ...

	printf("Login server -- recv packet\n");

}
