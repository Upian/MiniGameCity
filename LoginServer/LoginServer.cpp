#include "Login.h"
#include <string>

int main(void) {
	LoginServer* pServer = LoginServer::CreateServer();
	
	pServer->SetPortNum(Util::GetConfigToInt("LoginServer.ini", "Network", "Port"));
	pServer->InitializeBaseServer();
	pServer->RunServer();

	return 0;
}