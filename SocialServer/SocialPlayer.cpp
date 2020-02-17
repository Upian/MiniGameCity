#include "SocialPlayer.h"
#include "BasePacket.h"
#include "ClntServer.h"
#include "ErrorType.h"

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
	if (m_maxFriendListSize <= m_friendList.size())
		return ErrorTypeAddFriend::destFriendListIsFull;
	if (m_maxFriendRequestSize <= m_friendRequestList.size())
		return ErrorTypeAddFriend::destFriendRequestListIsFull;
	if (true == this->IsExistFriendRequestList(srcPlayer))
		return ErrorTypeAddFriend::alreadySendRequest;


	m_friendRequestList.emplace_back(srcPlayer->GetGPID(), srcPlayer->GetName());
	return ErrorTypeAddFriend::none;
}

bool SocialPlayer::IsExistFriendRequestList(std::shared_ptr<SocialPlayer> player) {
	for (auto p : m_friendRequestList) {
		if (player->GetGPID() == p.first)
			return true;
	}
	return false;
}

bool SocialPlayer::IsExistFriendRequestList(std::string name)
{
	for (auto p : m_friendRequestList) {
		if (name == p.second)
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
	m_friendList.emplace_back(player->GetGPID(), player->GetName());
	return true;
}

void SocialPlayer::DeleteFriendList(GPID gpid) {
	m_friendList.remove_if([&gpid](std::pair<GPID, std::string> p)->bool {
		return (gpid == p.first);
	});
}

void SocialPlayer::DeleteFriendRequestList(GPID gpid) {
	m_friendRequestList.remove_if([&gpid](std::pair<GPID, std::string> p)->bool {
		return (gpid == p.first);
	});
}

void SocialPlayer::DeleteFriendRequestList(std::string name) {
	m_friendRequestList.remove_if([&name](std::pair<GPID, std::string> p)->bool {
		return (name == p.second);
	});
}



