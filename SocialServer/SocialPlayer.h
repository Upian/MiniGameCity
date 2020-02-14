#pragma once
#ifndef __SOCIALSERVER_SOCIAL_PLAYER_H__
#define  __SOCIALSERVER_SOCIAL_PLAYER_H__

#include <list>
#include <memory>

#include "ErrorType.h"

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)

class Guild;
class ClntServer;
class BasePacket;


class SocialPlayer {
public:
	SocialPlayer() {}
	SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server);
	~SocialPlayer() {}

	void Initialize();

	void SendPacket(BasePacket& packet);

	GPID GetGPID() const { return m_gpid; }
	std::string GetName() const { return m_name; }

	std::shared_ptr<ClntServer> GetServer() const { return m_server; }
	std::shared_ptr<Guild> GetGuild() const { return m_guild; }

	//Friend
	bool IsFriendRequestEmpty() const { return m_friendRequestList.empty(); }
	std::list<std::shared_ptr<SocialPlayer> >& GetFriendRequestList() { return m_friendRequestList; }
	std::list<std::shared_ptr<SocialPlayer> >& GetFriendList() { return m_friendList; }
	ErrorTypeAddFriend AddFriendRequest(std::shared_ptr<SocialPlayer> pplayer);
	bool IsExistFriendRequestList(std::shared_ptr<SocialPlayer>);
	bool AddFriendList(std::shared_ptr<SocialPlayer>);
	bool IsAddFriendAvailable() { return m_maxFriendListSize > m_friendList.size(); }
	void DeleteFriendList(std::shared_ptr<SocialPlayer> player) { m_friendList.remove(player); }
	

	void SetName(std::string na) { m_name = na; } //#Test

private:
	// player info
	GPID m_gpid;
	std::shared_ptr<ClntServer> m_server = nullptr;
	std::string m_name;

	//Friend
	int m_maxFriendRequestSize = 5; //#DesignData
	int m_maxFriendListSize = 10; //#DesignData
	std::list<std::shared_ptr<SocialPlayer> > m_friendRequestList;
	std::list<std::shared_ptr<SocialPlayer> > m_friendList;
	
	//Guild
	std::shared_ptr<Guild> m_guild = nullptr;

};
#endif // !__SOCIALSERVER_SOCIAL_PLAYER_H__
