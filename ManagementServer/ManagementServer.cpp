#include "ManagementServer.h"

ManagementServer::ManagementServer() {}
ManagementServer::~ManagementServer() {}

/*
*	If a packet arrived
	It will come here
	Please Deserialize packet and handle each type
*/
void ManagementServer::HandleBasePacket(BufferInfo* bufInfo) {
	//DeSerialize bufInfo->dataBuf.buf

	//switch(packet type) ...

	printf("Management server -- recv packet: %s\n", bufInfo->dataBuf.buf);

}

void ManagementServer::HandleAcceptClient(SOCKET clientSocket) {
	

}