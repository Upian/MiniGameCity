#include "SocialServer.h"

int main(int argc, char* argv[]) {
	SocialServer* pServer = SocialServer::CreateServer();


	if (1 < argc) //set config file
		pServer->GetConfigManager()->SetDirectory(argv[1]);

	pServer->SetPortNum(Util::GetConfigToInt("SocialServer.ini", "Network", "Port", 20000));
	pServer->SetServerName("SocialServer");

	pServer->InitializeBaseServer();
	pServer->InitializeSocialServer();

	pServer->RunServer();


	return 0;
}