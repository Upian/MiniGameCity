#include "ManagementServer.h"

int main(void) {

	ManagementServer* pServer = ManagementServer::CreateServer();

	pServer->SetPortNum(Util::GetConfigToInt("ManagementServer.ini", "Network", "Port", 19999));
	pServer->SetServerName("ManagementServer");
	pServer->InitializeBaseServer();

	pServer->RunServer();

	getchar();
	return 0;
}