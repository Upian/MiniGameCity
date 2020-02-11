#include "SocialPlayerManager.h"
#include "SocialPlayer.h"
#include "ClntServer.h"

void SocialPlayerManager::InsertPlayer(GPID gpid, SOCKET socket) {
	if (true == this->IsExistSocialPlayer(gpid))
		return;


}
void SocialPlayerManager::InsertPlayer(GPID gpid, ClntServer server) {
	
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
