#include "RankingServer.h"

RankingServer::RankingServer() {}
RankingServer::~RankingServer() {}


/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void RankingServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf
	
	//switch(packet type) ...

	printf("Ranking server -- recv packet\n");

}

void RankingServer::HandleAcceptClient(SOCKET clientSocket) {

}