#include "ClntServerManager.h"

void ClntServerManager::InsertServer(SOCKET socket) {
	if (true == this->isExistServer(socket)) 
		return;
	
	std::shared_ptr<ClntServer> temp = std::make_shared<ClntServer>(socket);
	m_servers.push_back(temp);
}

bool ClntServerManager::isExistServer(SOCKET socket) {
	for (auto s : m_servers) {
		if (socket == s->GetSocket())
			return true;
	}
	return false;
}

bool ClntServerManager::isExistServer(std::shared_ptr<ClntServer> player) {
	for (auto s : m_servers) {
		if (player == s) 
			return true;
	}
	return false;
}
