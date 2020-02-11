#pragma once
#ifndef __SOCIALSERVER_SOCIAL_PLAYER_H__
#define  __SOCIALSERVER_SOCIAL_PLAYER_H__

#include <list>
#include <memory>

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)

class Guild;
class ClntServer;

class SocialPlayer {
public:
	SocialPlayer() {}
	SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server);
	~SocialPlayer();

	GPID GetGPID() const { return m_gpid; }
	std::shared_ptr<ClntServer> GetServer() const { return m_server; }
	std::shared_ptr<Guild> GetGuild() const { return m_guild; }

private:
	// player info
	GPID m_gpid;
	std::shared_ptr<ClntServer> m_server = nullptr;


	//Friend
	std::list<std::shared_ptr<SocialPlayer> > m_friendList;
	
	//Guild
	std::shared_ptr<Guild> m_guild = nullptr;

};
#endif // !__SOCIALSERVER_SOCIAL_PLAYER_H__
