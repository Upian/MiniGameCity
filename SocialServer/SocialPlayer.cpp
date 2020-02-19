#include "SocialServer.h"
#include "SocialPlayer.h"
#include "BasePacket.h"
#include "ClntServer.h"
#include "ErrorType.h"

using FriendInfo = std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >;

SocialPlayer::SocialPlayer(GPID gpid, std::shared_ptr<ClntServer> server) : 
	m_gpid(gpid), 
	m_server(server) {

}

void SocialPlayer::Initialize() {
	//Load from database
	//friend, guild request
}

void SocialPlayer::SendPacket(BasePacket & packet) {
	this->GetServer()->SendPacket(packet); 
}

ErrorTypeAddFriend SocialPlayer::AddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer) {
	if (nullptr == srcPlayer)
		return ErrorTypeAddFriend::notExistPlayer;
	if (true == this->IsExistFriendList(srcPlayer))
		return ErrorTypeAddFriend::alreadyFriends;
	if (m_maxFriendListSize <= m_friendList.size())
		return ErrorTypeAddFriend::destFriendListIsFull;
	if (m_maxFriendRequestSize <= m_friendRequestList.size())
		return ErrorTypeAddFriend::destFriendRequestListIsFull;
	if (true == this->IsExistFriendRequestList(srcPlayer))
		return ErrorTypeAddFriend::alreadySendRequest;
	if (m_gpid == srcPlayer->GetGPID())
		return ErrorTypeAddFriend::samePlayer;
	if (true == this->IsExistFriendRequestList(srcPlayer))
		return ErrorTypeAddFriend::alreadySendRequest;

	m_friendRequestList.emplace_back(srcPlayer->GetGPID(), srcPlayer->GetName(), srcPlayer);
	return ErrorTypeAddFriend::none;
}

bool SocialPlayer::IsExistFriendRequestList(std::shared_ptr<SocialPlayer> player) {
	for (auto p : m_friendRequestList) {
		if (player->GetGPID() == GetFriendGPIDInfo(p))
			return true;
	}
	return false;
}

bool SocialPlayer::IsExistFriendRequestList(std::string name) {
	for (auto p : m_friendRequestList) {
		if (name == GetFriendNameInfo(p))
			return true;
	}
	return false;
}

bool SocialPlayer::IsExistFriendList(std::shared_ptr<SocialPlayer> player) {
	for (FriendInfo& p : m_friendList) {
		if (player->GetGPID() == GetFriendGPIDInfo(p))
			return true;
	}
	return false;
}

bool SocialPlayer::IsExistFriendList(std::string name) {
	for (auto p : m_friendList) {
		if (name == GetFriendNameInfo(p))
			return true;
	}
	return false;
}

bool SocialPlayer::AddFriendList(std::shared_ptr<SocialPlayer> player) {
	if (m_maxFriendListSize <= m_friendList.size())
		return false;
	if (nullptr == player)
		return false;

	this->DeleteFriendRequestList(player->GetGPID());
	m_friendList.emplace_back(player->GetGPID(), player->GetName(), player);
	return true;
}
//Call when player login
void SocialPlayer::InformLoginToFriends() {
	//Find loggedin player
	auto manager = SocialServer::GetServer()->GetSocialPlayerManager();
	for (auto p : m_friendList) {
		std::shared_ptr<SocialPlayer> friendPlayer = manager.FindSocialPlayer(GetFriendGPIDInfo(p));
		if (nullptr == friendPlayer)
			continue;
		//Let my friends know I'm logged in
		friendPlayer->UpdateFriendIsLogin(shared_from_this());

		//Update logged in player
		GetFriend(p) = friendPlayer;
	}
}

void SocialPlayer::UpdateFriendIsLogin(std::shared_ptr<SocialPlayer> player) {
	if (nullptr == player)
		return;

	for (auto p : m_friendList) {
		if (player->GetGPID() == GetFriendGPIDInfo(p)) {
			GetFriend(p) = player;
			return;
		}
	}

}

void SocialPlayer::DeleteFriendList(GPID gpid) {
	m_friendList.remove_if([this, &gpid](FriendInfo& p)->bool {
		return (gpid == GetFriendGPIDInfo(p));
	});
}

void SocialPlayer::DeleteFriendRequestList(GPID gpid) {
	m_friendRequestList.remove_if([this, &gpid](FriendInfo& p)->bool {
		return (gpid == GetFriendGPIDInfo(p));
	});
}

void SocialPlayer::DeleteFriendRequestList(std::string name) {
	m_friendRequestList.remove_if([this, &name](FriendInfo& p)->bool {
		return (name == GetFriendNameInfo(p));
	});
}

GPID GetFriendGPIDInfo(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer>>& info) {
	return std::get<0>(info);
}
std::string GetFriendNameInfo(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >& info) {
	 return std::get<1>(info); 
}
std::shared_ptr<SocialPlayer> GetFriend(const std::tuple<GPID, std::string, std::shared_ptr<SocialPlayer> >& info) {
	return std::get<2>(info); 
}
