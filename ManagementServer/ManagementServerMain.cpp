#include "ManagementServer.h"

int main(void) {

	ManagementServer* pServer = ManagementServer::CreateServer();

	pServer->SetPortNum(Util::GetConfigToInt("ManagementServer.ini", "Network", "Port", 19999));
	pServer->SetServerName("Management server");
	pServer->InitializeBaseServer();
	pServer->RunServer();

	getchar();
	return 0;
}