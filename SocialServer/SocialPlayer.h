#pragma once
#ifndef __SOCIALSERVER_SOCIAL_PLAYER_H__
#define  __SOCIALSERVER_SOCIAL_PLAYER_H__

#include <list>
#include <memory>
#include <queue>
using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)

class Guild;
class ClntServer;

class SocialPlayer {
public:
	SocialPlayer() {}
	SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server);
	~SocialPlayer() {}

	void Initialize();

	GPID GetGPID() const { return m_gpid; }
	std::string GetName() const { return m_name; }

	std::shared_ptr<ClntServer> GetServer() const { return m_server; }
	std::shared_ptr<Guild> GetGuild() const { return m_guild; }

	//Friend
	bool IsFriendRequestEmpty() const { return m_friendRequestQueue.empty(); }
	std::shared_ptr<SocialPlayer> GetFriendRequest() { m_friendRequestQueue.front(); m_friendRequestQueue.pop(); }
	void AddFriendRequest(std::shared_ptr<SocialPlayer> pplayer) { m_friendRequestQueue.push(pplayer); }

	void SetName(std::string na) { m_name = na; } //#Test

private:
	// player info
	GPID m_gpid;
	std::shared_ptr<ClntServer> m_server = nullptr;
	std::string m_name;

	//Friend
	std::queue<std::shared_ptr<SocialPlayer> > m_friendRequestQueue;
	std::list<std::shared_ptr<SocialPlayer> > m_friendList;
	
	//Guild
	std::shared_ptr<Guild> m_guild = nullptr;

};
#endif // !__SOCIALSERVER_SOCIAL_PLAYER_H__
