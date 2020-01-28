#include "GameServer.h"

int main(void) {
	GameServer* pServer = GameServer::CreateServer();
	
	pServer->SetPortNum(Util::GetConfigToInt("GameServer.ini", "Network", "Port", 7979));
	pServer->SetServerName("GameServer");

	pServer->InitializeBaseServer();
	pServer->InitializeGameServer();

	pServer->RunServer();
	//getchar();
	return 0;
}
