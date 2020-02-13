#pragma once
#ifndef __SOCIALSERVER_FRIENDS_MANAGER_H__
#define __SOCIALSERVER_FRIENDS_MANAGER_H__

#include <memory>

class SocialPlayer;

class FriendsManager {
public:
	//FriendsManager();
	void HandleAddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, std::shared_ptr<SocialPlayer> destPlayer);
	void HandleConfirmFriendRequest(std::shared_ptr<SocialPlayer> pplayer);
	void HandleAcceptFriendReequest(std::shared_ptr<SocialPlayer>, std::shared_ptr<SocialPlayer>);

private:

	unsigned int m_friendTicketNum;

};


#endif // !__SOCIALSERVER_FRIENDS_MANAGER_H__