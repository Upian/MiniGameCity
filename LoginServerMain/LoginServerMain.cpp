#include "LoginServer.h"

int main(int argc, char* argv[]) {
	LoginServer* pServer = LoginServer::CreateServer();

	/*if (0 < argc)
		pServer->SetPortNum(atoi(argv[1]));
	else*/
	pServer->SetPortNum(Util::GetConfigToInt("LoginServer.ini", "Network", "Port", 30000)); // 10001
	pServer->SetServerName("LoginServer");

	pServer->InitializeBaseServer();
	pServer->InitializeLoginServer();

	pServer->RunServer();

	return 0;
}