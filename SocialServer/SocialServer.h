#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_H__
#define __SOCIALSERVER_SOCIAL_SERVER_H__
#include "BaseServer.h"
#include "ClntServerManager.h"
#include "SocialPlayerManager.h"

class SocialServer : public BaseServer<SocialServer> {
public:
	void InitializeSocialServer();

private:
	MAKE_SERVER(SocialServer);

	void HandleBaseSocialPacket(Buffer& buffer);
	void LoadPlayerSocialData(GPID gpid);
	ClntServerManager m_gameServers;
	SocialPlayerManager m_socialPlayerManager;
};


#endif // !__SOCIALSERVER_SOCIAL_SERVER_H__
