#include "LoginServer.h"

int main(int argc, char* argv[]) {
	LoginServer* pServer = LoginServer::CreateServer();

	if (1 < argc)
		pServer->GetConfigManager()->SetDirectory(argv[1]);
	
	pServer->SetPortNum(Util::GetConfigToInt("LoginServer.ini", "Network", "Port", 10000));
	pServer->SetServerName("LoginServer");

	pServer->InitializeBaseServer();
	pServer->InitializeLoginServer();

	pServer->RunServer();

	return 0;
}