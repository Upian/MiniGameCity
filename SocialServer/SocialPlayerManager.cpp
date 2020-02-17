#include "SocialPlayerManager.h"
#include "SocialPlayer.h"
#include "ClntServer.h"

std::shared_ptr<SocialPlayer> SocialPlayerManager::InsertPlayer(GPID gpid, std::shared_ptr<ClntServer> server) {
	if (true == this->IsExistSocialPlayer(gpid))
		return nullptr;

	std::shared_ptr<SocialPlayer> temp(new SocialPlayer(gpid, server));

	m_socialPlayers.push_back(temp);
	return temp;
}

void SocialPlayerManager::RemovePlayer(GPID gpid) {
	auto pplayer = this->FindSocialPlayer(gpid);
	if (nullptr == pplayer)
		return;

	m_socialPlayers.remove(pplayer);
	
}

std::shared_ptr<SocialPlayer> SocialPlayerManager::FindSocialPlayer(GPID gpid) {
	for (auto p : m_socialPlayers) {
		if (gpid == p->GetGPID())
			return p;
	}

	//#DatabaseFind
	return nullptr;
}

std::shared_ptr<SocialPlayer> SocialPlayerManager::FindSocialPlayer(std::string name) {
	for (auto p : m_socialPlayers) {
		if (name == p->GetName()) 
			return p;
	}
	//#DatabaseFind
	return nullptr;
}

bool SocialPlayerManager::IsExistSocialPlayer(GPID gpid)
{
	for (auto p : m_socialPlayers) {
		if (gpid == p->GetGPID())
			return true;
	}

	return false;
}
