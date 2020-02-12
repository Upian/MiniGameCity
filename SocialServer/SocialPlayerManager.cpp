#include "SocialPlayerManager.h"
#include "SocialPlayer.h"
#include "ClntServer.h"

std::shared_ptr<SocialPlayer> SocialPlayerManager::InsertPlayer(GPID gpid, SOCKET socket) {
	if (true == this->IsExistSocialPlayer(gpid))
		return nullptr;
}
std::shared_ptr<SocialPlayer> SocialPlayerManager::InsertPlayer(GPID gpid, std::shared_ptr<ClntServer> server) {
	if (true == this->IsExistSocialPlayer(gpid))
		return nullptr;

	std::shared_ptr<SocialPlayer> temp(new SocialPlayer(gpid, server));

	m_socialPlayers.push_back(temp);
	return temp;
}

std::shared_ptr<SocialPlayer> SocialPlayerManager::FindSocialPlayer(GPID gpid)
{
	for (auto p : m_socialPlayers) {
		if (gpid == p->GetGPID())
			return p;
	}
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
