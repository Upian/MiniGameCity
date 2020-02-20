#include "LoginDBCache.h"

int main(int argc, char* argv[]) {
    LoginDBCache* pServer = LoginDBCache::CreateServer();

	if (1 < argc)
		pServer->SetPortNum(atoi(argv[1]));
	else
		pServer->SetPortNum(Util::GetConfigToInt("DBCache.ini", "Network", "Port", 30001));
	pServer->SetServerName("LoginDBCache");

	pServer->InitializeBaseServer();

	pServer->RunServer();

	return 0;
}