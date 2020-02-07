#include "RankingServer.h"

int main(void) {
	RankingServer::CreateServer();
	RankingServer* pServer = RankingServer::GetServer();

	pServer->SetPortNum(Util::GetConfigToInt("RankingServer.ini", "Network", "Port", 7979));
	pServer->SetServerName("Ranking server");
	pServer->InitializeBaseServer();
	pServer->RunServer();

	return 0;
}