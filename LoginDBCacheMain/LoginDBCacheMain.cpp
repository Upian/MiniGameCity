#include "LoginDBCache.h"

int main(int argc, char* argv[]) {
    LoginDBCache* pServer = LoginDBCache::CreateServer();

	if (1 < argc) 
		pServer->GetConfigManager()->SetDirectory(argv[1]);
	
	pServer->SetPortNum(Util::GetConfigToInt("LoginDBCache.ini", "Network", "Port", 30000));
	pServer->SetServerName("LoginDBCache");

	pServer->InitializeBaseServer();

	pServer->RunServer();
	return 0;
}