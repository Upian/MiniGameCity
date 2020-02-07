#include "ManagementServer.h"

int main(int argc, char* argv[]) {
	ManagementServer* pServer = ManagementServer::CreateServer();

	if (1 < argc) //set config file
		pServer->GetConfigManager()->SetDirectory(argv[1]);

	pServer->SetPortNum(Util::GetConfigToInt("ManagementServer.ini", "Network", "Port", 19999));
	pServer->SetServerName("ManagementServer");
	
	pServer->InitializeBaseServer();
	
	
	pServer->RunServer();

	getchar();
	return 0;
}