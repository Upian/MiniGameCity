#include "DBCache.h"

int main(int argc, char* argv[]) {
	DBCache* pServer = DBCache::CreateServer();

	if (1 < argc)
		pServer->SetPortNum(atoi(argv[1]));
	else
		pServer->SetPortNum(Util::GetConfigToInt("DBCache.ini", "Network", "Port", 30000));
	pServer->SetServerName("DBCache");

	pServer->InitializeBaseServer();

	pServer->RunServer();

	return 0;
}