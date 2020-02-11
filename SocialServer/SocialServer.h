#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_H__
#define __SOCIALSERVER_SOCIAL_SERVER_H__
#include "BaseServer.h"
#include "ClntServerManager.h"

class SocialServer : public BaseServer<SocialServer> {
public:
	void InitializeSocialServer();

private:
	MAKE_SERVER(SocialServer);

	void HandleBaseSocialPacket(Buffer& buffer);
	ClntServerManager m_gameServers;
};


#endif // !__SOCIALSERVER_SOCIAL_SERVER_H__
