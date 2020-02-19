#pragma once
#ifndef __SOCIALSERVER_SOCIAL_PLAYER_H__
#define  __SOCIALSERVER_SOCIAL_PLAYER_H__

#include <list>
#include <memory>
#include <tuple>

#include "ErrorType.h"

using GPID = unsigned __int32; //Unique ID for each player (Game Player ID)


class Guild;
class ClntServer;
class BasePacket;

class SocialPlayer : std::enable_shared_from_this<SocialPlayer> {
private:
	using FriendInfo = std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >;
public:
	SocialPlayer() {}
	SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server);
	~SocialPlayer() {} //#DatabaseSave

	void Initialize();

	void SendPacket(BasePacket& packet);

	GPID GetGPID() const { return m_gpid; }
	std::string GetName() const { return m_name; }

	std::shared_ptr<ClntServer> GetServer() const { return m_server; }
	std::shared_ptr<Guild> GetGuild() const { return m_guild; }

	//Friend
	bool IsFriendRequestEmpty() const { return m_friendRequestList.empty(); }
	std::list<FriendInfo>& GetFriendRequestList() { return m_friendRequestList; }
	std::list<FriendInfo>& GetFriendList() { return m_friendList; }

	ErrorTypeAddFriend AddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer); //Only login player can add friend request
	bool IsExistFriendRequestList(std::shared_ptr<SocialPlayer>);
	bool IsExistFriendRequestList(std::string);
	bool IsExistFriendList(std::shared_ptr<SocialPlayer>);
	bool IsExistFriendList(std::string);

	bool AddFriendList(std::shared_ptr<SocialPlayer>);
	bool IsAddFriendAvailable() { return m_maxFriendListSize > m_friendList.size(); }
	void InformLoginToFriends();
	void UpdateFriendIsLogin(std::shared_ptr<SocialPlayer>);

	void DeleteFriendList(GPID);
	void DeleteFriendRequestList(GPID);
	void DeleteFriendRequestList(std::string);

	void SetName(std::string na) { m_name = na; } //#Test

private:
	// player info
	GPID m_gpid;
	std::shared_ptr<ClntServer> m_server = nullptr;
	std::string m_name;

	//Friend
	int m_maxFriendRequestSize = 5; //#DesignData
	int m_maxFriendListSize = 10; //#DesignData
	std::list<FriendInfo> m_friendRequestList;
	std::list<FriendInfo> m_friendList;
	
	//Guild
	std::shared_ptr<Guild> m_guild = nullptr;

};


GPID GetFriendGPIDInfo(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >& info);
std::string GetFriendNameInfo(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >& info);
std::shared_ptr<SocialPlayer> GetFriend(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >& info);




#endif // !__SOCIALSERVER_SOCIAL_PLAYER_H__
