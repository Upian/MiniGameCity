#pragma once
#ifndef __SOCIALSERVER_FRIENDS_MANAGER_H__
#define __SOCIALSERVER_FRIENDS_MANAGER_H__

#include <memory>
#include "SocialServerPacket.h"

class SocialPlayer;

class FriendsManager {
public:
	//FriendsManager();
	void HandleAddFriendRequest(std::shared_ptr<SocialPlayer> , std::string);
	void HandleConfirmFriendRequest(std::shared_ptr<SocialPlayer> pplayer);
	void HandleAcceptFriendReequest(std::shared_ptr<SocialPlayer>, std::string, bool);
	void HandleFriendListRequest(std::shared_ptr<SocialPlayer>);
	void HandleDeleteFriendRequest(std::shared_ptr<SocialPlayer>, std::string);
	void HandleChatFriendRequest(std::shared_ptr<SocialPlayer>, const std::string& destName, const std::string& message);
	void HandleInviteFriendRequest(std::shared_ptr<SocialPlayer>, SocialPacketServerInviteFriendRequest& packet);

private:

	unsigned int m_friendTicketNum;

};


#endif // !__SOCIALSERVER_FRIENDS_MANAGER_H__
