#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_H__
#define __SOCIALSERVER_SOCIAL_SERVER_H__
#include "BaseServer.h"

class SocialServer : public BaseServer<SocialServer> {
public:
	void InitializeSocialServer();

private:
	MAKE_SERVER(SocialServer)

};


#endif // !__SOCIALSERVER_SOCIAL_SERVER_H__
