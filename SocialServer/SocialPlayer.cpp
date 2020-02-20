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

	m_friendRequestList.emplace_back(srcPlayer->GetGPID(), srcPlayer->GetName(), true, srcPlayer);
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
	for (FriendInfo& p : this->GetFriendList()) {
		if (player->GetGPID() == GetFriendGPIDInfo(p))
			return true;
	}
	return false;
}

bool SocialPlayer::IsExistFriendList(std::string name) {
	for (auto p : this->GetFriendList()) {
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
	m_friendList.emplace_back(player->GetGPID(), player->GetName(), true, player);
	return true;
}
//Call when player login
void SocialPlayer::InformLoginToFriends() {
	//Find loggedin player
	for (auto p : this->GetFriendList()) {
		if (false == GetFriendIsLoginInfo(p))
			return;

		auto friendPlayer = GetFriend(p);
		if (nullptr == friendPlayer)
			return;
		//Let my friends know I'm logged in
		friendPlayer->UpdateFriendIsLogin(shared_from_this());

		//#update my friendList

	}
}

void SocialPlayer::InformLogoutToFriends() {
	//this -> logout player
	//Find loggedin player
	for (auto loginPlayer : this->GetFriendList()) {
		if (false == GetFriendIsLoginInfo(loginPlayer))
			return;

		auto friendPlayer = GetFriend(loginPlayer);
		if (nullptr == friendPlayer)
			return;
		//Let my friends know I'm logged out
		friendPlayer->UpdateFriendIsLogout(shared_from_this());

	}
}

void SocialPlayer::UpdateFriendIsLogin(std::shared_ptr<SocialPlayer> player) {
	if (nullptr == player)
		return;

	for (auto p : this->GetFriendList()) {
		if (player->GetGPID() == GetFriendGPIDInfo(p)) {
			SetFriendIsLoginInfo(p, true);
			SetFriend(p, player);
			return;
		}
	}
}

void SocialPlayer::UpdateFriendIsLogout(std::shared_ptr<SocialPlayer> player) {
	if (nullptr == player)
		return;

	for (auto& p : this->GetFriendList()) {
		if (player->GetGPID() == GetFriendGPIDInfo(p)) {
			SetFriendIsLoginInfo(p, false);
			SetFriend(p, nullptr);
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

GPID GetFriendGPIDInfo(const std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer>>& info) {
	return std::get<0>(info);
}
std::string GetFriendNameInfo(const std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer> >& info) {
	 return std::get<1>(info); 
}
bool GetFriendIsLoginInfo(std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer> >& info) {
	return std::get<2>(info);
}

std::shared_ptr<SocialPlayer> GetFriend(std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer>>& info) {
	bool isLogin = GetFriendIsLoginInfo(info);
	auto temp = std::get<3>(info);
	if (false == isLogin) {
		if (nullptr == temp)
			return nullptr;
		temp.reset();
		temp = nullptr;
	}
	return temp;
}

void SetFriendIsLoginInfo(std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer>>& info, const bool isLogin) {
	if (false == isLogin) {
		std::get<3>(info).reset();
		std::get<3>(info) = nullptr;
	}
	std::get<2>(info) = isLogin;
}

void SetFriend(std::tuple<GPID, std::string, bool, std::shared_ptr<SocialPlayer>>& info, std::shared_ptr<SocialPlayer> player) {
	if (nullptr == player) {
		std::get<3>(info).reset();
		std::get<3>(info) = nullptr;
		return;
	}
	std::get<3>(info) = player;
}
