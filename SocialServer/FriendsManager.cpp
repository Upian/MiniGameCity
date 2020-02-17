#include <string>
#include "FriendsManager.h"
#include "SocialPlayer.h"
#include "ClntServer.h"
#include "SocialServerPacket.h"
#include "Log.h"
#include "SocialServer.h"

void FriendsManager::HandleAddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, std::string destName) {
	if (nullptr == srcPlayer) 
		return;

	auto destPlayer = SocialServer::GetServer()->GetSocialPlayerManager().FindSocialPlayer(destName);
	if (nullptr == destPlayer) { //if dest players state is logout
		//#DatabaseLoad
		//Handle database
		//make destplayer pointer and set GPID and name
		Util::LoggingDebug("Friends.log", "AddFriendRequest-> DestPlayer is not logged in, So check database and handle it");
		return;
	}

	//dest player state is login
	SocialPacketServerAddFriendResponse packet;
	packet.m_gpid = srcPlayer->GetGPID();
	packet.m_success = true;
	packet.m_errorType = destPlayer->AddFriendRequest(srcPlayer);
	
	if (ErrorTypeAddFriend::none != packet.m_errorType)
		packet.m_success = false;
	
	Util::LoggingDebug("Friends.log", "srcPlayer[%d, %s] wants to friends with destPlayer[%d, %s] is %s",
		srcPlayer->GetGPID(), srcPlayer->GetName().c_str(),
		destPlayer->GetGPID(), destPlayer->GetName().c_str(),
		[&packet]()->const char*{
			if (ErrorTypeAddFriend::none == packet.m_errorType)
				return "Success";
			else if (ErrorTypeAddFriend::alreadySendRequest == packet.m_errorType)
				return "already send request";
			else if (ErrorTypeAddFriend::samePlayer == packet.m_errorType)
				return "request same player";
	}());
	srcPlayer->GetServer()->SendPacket(packet);
}

void FriendsManager::HandleConfirmFriendRequest(std::shared_ptr<SocialPlayer> pplayer) {
	if (nullptr == pplayer)
		return;
	if (true == pplayer->IsFriendRequestEmpty())
		return;

	SocialPacketServerConfirmFriendResponse packet;
	packet.m_gpid = pplayer->GetGPID();
	for (auto p : pplayer->GetFriendRequestList()) {
		packet.m_names.emplace_back(p.second);
	}

	pplayer->GetServer()->SendPacket(packet);
}

void FriendsManager::HandleAcceptFriendReequest(std::shared_ptr<SocialPlayer> src, std::string dest, bool isAccept) {
	if (nullptr == src) //
		return;
	if (src->GetName() == dest)
		return;
	if (false == src->IsExistFriendRequestList(dest))
		return;

	if (false == isAccept) {
		src->DeleteFriendRequestList(dest);
		return;
	}
		

	auto destPlayer = SocialServer::GetServer()->GetSocialPlayerManager().FindSocialPlayer(dest);
	if (nullptr == destPlayer) {
		//#DatabaseLoad Handle
		Util::LoggingDebug("Friends.log", "AcceptFriendRequest-> DestPlayer is not logged in, So check database and handle it");
		return;
	}

	//Player is exist in friendRequestList and logged in
	SocialPacketServerAcceptFriendResponse packet;
	packet.m_gpid = src->GetGPID();
	packet.m_isSuccess = true;
	packet.m_errorCode = ErrorTypeAcceptFriend::none;

	if (false == src->AddFriendList(destPlayer)) {
		packet.m_errorCode = ErrorTypeAcceptFriend::srcFriendListIsFull;
		packet.m_isSuccess = false;
	}
	else if (false == destPlayer->AddFriendList(src)) {
		packet.m_errorCode = ErrorTypeAcceptFriend::destFriendListIsFull;
		packet.m_isSuccess = false;
	}

	if (false == packet.m_isSuccess) { //둘 중 하나라도 친구 추가 실패 하면
		src->DeleteFriendList(destPlayer->GetGPID());
		destPlayer->DeleteFriendList(src->GetGPID());
	}

	src->GetServer()->SendPacket(packet);
}

void FriendsManager::HandleFriendListRequest(std::shared_ptr<SocialPlayer> player) {
	SocialPacketServerFriendListResponse packet;
	packet.m_gpid = player->GetGPID();
	for (auto p : player->GetFriendList()) {
		packet.m_names.emplace_back(p.second);
	}
	
	player->GetServer()->SendPacket(packet);
}

void FriendsManager::HandleDeleteFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, std::string deleteFriend) {
	if (nullptr == srcPlayer)
		return;
	if (srcPlayer->GetName() == deleteFriend)
		return;

	auto deletePlayer = SocialServer::GetServer()->GetSocialPlayerManager().FindSocialPlayer(deleteFriend);
	if (nullptr == deletePlayer) {
		//#DatabaseLoad Handle
		Util::LoggingDebug("Friends.log", "DeleteFriendRequest-> DestPlayer is not logged in, So check database and handle it");
	}

	SocialPacketServerDeleteFriendResponse responsePacket;
	srcPlayer->DeleteFriendList(deletePlayer->GetGPID());
	deletePlayer->DeleteFriendList(deletePlayer->GetGPID());

	responsePacket.m_isSuccess = true;

	srcPlayer->GetServer()->SendPacket(responsePacket);
}
