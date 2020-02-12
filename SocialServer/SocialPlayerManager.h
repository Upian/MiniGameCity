#pragma once
#ifndef __SOCIALSERVER_SOCIAL_PLAYER_MANAGER_H__
#define  __SOCIALSERVER_SOCIAL_PLAYER_MANAGER_H__

#include <list>

#include "ServerCommon.h"

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)
class SocialPlayer;
class ClntServer;

class SocialPlayerManager {
public:
	std::shared_ptr<SocialPlayer> InsertPlayer(GPID gpid, SOCKET socket);
	std::shared_ptr<SocialPlayer> InsertPlayer(GPID gpid, std::shared_ptr<ClntServer> server);

	std::list<std::shared_ptr<SocialPlayer> >& GetSocialPlayerList() { return m_socialPlayers; }

	std::shared_ptr<SocialPlayer> FindSocialPlayer(GPID gpid);
	bool IsExistSocialPlayer(GPID gpid);
private:
	std::list<std::shared_ptr<SocialPlayer> > m_socialPlayers;

};

#endif // !__SOCIALSERVER_SOCIAL_PLAYER_MANAGER_H__
