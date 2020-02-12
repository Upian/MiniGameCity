#include "FriendsManager.h"
#include "SocialPlayer.h"

void FriendsManager::HandleAddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, std::shared_ptr<SocialPlayer> destPlayer) {
	if (nullptr == srcPlayer)
		return;


	if (nullptr == destPlayer) {
		//#DatabaseLoad
		//Handle database
		return;
	}

	destPlayer->AddFriendRequest(srcPlayer);
}
