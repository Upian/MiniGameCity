#include "Login.h"

int main(void) {
	LoginServer* pServer = LoginServer::CreateServer();

	pServer->SetPortNum(Util::GetConfigToInt("LoginServer.ini", "Network", "Port"));
	pServer->SetServerName("Login server");
	pServer->InitializeBaseServer();
	pServer->RunServer();

	return 0;
}