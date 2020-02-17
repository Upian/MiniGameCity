#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_H__
#define __SOCIALSERVER_SOCIAL_SERVER_H__
#include "BaseServer.h"
#include "ClntServerManager.h"
#include "SocialPlayerManager.h"
#include "SocialServerPacket.h"
#include "FriendsManager.h"

class SocialServer : public BaseServer<SocialServer> {
public:
	void InitializeSocialServer();

	SocialPlayerManager& GetSocialPlayerManager() { return m_socialPlayerManager; }
	FriendsManager& GetFriendManager() { return m_friendManager; }
private:
	MAKE_SERVER(SocialServer);

	void HandleBaseSocialPacket(BufferInfo* bufInfo);
	void HandleUpdatePlayerLogin(SocialPacketServerUpdatePlayerLogin&, std::shared_ptr<ClntServer>);
	void HandleUpdatePlayerLogout(SocialPacketServerUpdatePlayerLogout&, std::shared_ptr<ClntServer>);
	void LoadPlayerSocialData(std::shared_ptr<SocialPlayer>);
	ClntServerManager m_gameServers;
	SocialPlayerManager m_socialPlayerManager;

	//
	FriendsManager m_friendManager;
};


#endif // !__SOCIALSERVER_SOCIAL_SERVER_H__
