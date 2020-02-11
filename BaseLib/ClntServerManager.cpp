#include "ClntServerManager.h"

void ClntServerManager::InsertServer(SOCKET socket) {
	if (true == this->isExistServer(socket)) 
		return;
	
//	std::shared_ptr<ClntServer> temp = std::make_shared<ClntServer>(socket);
	m_servers.emplace_back(new ClntServer(socket));
//	m_servers.push_back(temp);
}

void ClntServerManager::RemoveServer(SOCKET socket) {
	auto pserver = this->FindServerBySocket(socket);
	if (nullptr == pserver)
		return;
	m_servers.remove(pserver);
}

std::shared_ptr<ClntServer> ClntServerManager::FindServerBySocket(SOCKET socket) {
	for (auto s : m_servers) {
		if (socket == s->GetSocket())
			return s;
	}
	return nullptr;
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
