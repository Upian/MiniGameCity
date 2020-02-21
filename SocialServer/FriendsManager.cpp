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
		Util::LoggingInfo("Friends.log", "AddFriendRequest-> DestPlayer is not logged in, So check database and handle it");
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
		switch (packet.m_errorType) {
		case ErrorTypeAddFriend::none: 
			return "Success";
		case ErrorTypeAddFriend::alreadySendRequest:
			return "already send request";
		case ErrorTypeAddFriend::samePlayer:
			return "request same player";
		case ErrorTypeAddFriend::alreadyFriends: {
			return "already friend";
		case ErrorTypeAddFriend::destFriendListIsFull: 
		case ErrorTypeAddFriend::srcFriendListIsFull:
			return "friendList is full";
		default:
			return "UNKNOWN error";
		}
		}
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
		packet.m_names.emplace_back(GetFriendNameInfo(p));
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
		Util::LoggingInfo("Friends.log", "AcceptFriendRequest-> DestPlayer is not logged in, So check database and handle it");
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
	if (nullptr == player)
		return;
	SocialPacketServerFriendListResponse packet;
	packet.m_gpid = player->GetGPID();
	for (auto p : player->GetFriendList()) {
		packet.m_friends.emplace_back(GetFriendNameInfo(p), GetFriendIsLoginInfo(p));
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
		Util::LoggingInfo("Friends.log", "DeleteFriendRequest-> DestPlayer is not logged in, So check database and handle it");
		return;
	}

	SocialPacketServerDeleteFriendResponse responsePacket;
	srcPlayer->DeleteFriendList(deletePlayer->GetGPID());
	deletePlayer->DeleteFriendList(srcPlayer->GetGPID());
	
	responsePacket.m_gpid = srcPlayer->GetGPID();
	responsePacket.m_isSuccess = true;

	srcPlayer->GetServer()->SendPacket(responsePacket);
}

void FriendsManager::HandleChatFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, const std::string& destName, const std::string& message) {
	if (nullptr == srcPlayer)
		return;

	if (false == srcPlayer->IsExistFriendList(destName)) //Not a friend, Fail
		return;

	auto destPlayer = SocialServer::GetServer()->GetSocialPlayerManager().FindSocialPlayer(destName);
	if (nullptr == destPlayer) {
		//#DatabaseLoad Handle
		Util::LoggingInfo("Friends.log", "ChatFriendRequest-> DestPlayer is not logged in, So check database and handle it");
		return;
	}

	SocialPacketServerChatFriendResponse responseSrc;
	SocialPacketServerChatFriendResponse responseDest;
	responseSrc.m_gpid = srcPlayer->GetGPID();
	responseSrc.m_name = destPlayer->GetName();
	responseSrc.m_message = message;
	responseSrc.m_isSender = true;

	responseDest.m_gpid = destPlayer->GetGPID();
	responseDest.m_name = srcPlayer->GetName();
	responseDest.m_message = message;
	responseDest.m_isSender = false;

	Util::LoggingDebug("Friends.log", "%s[%d] send message to %s[%d], Message: %s",
		srcPlayer->GetName().c_str(), srcPlayer->GetGPID(),
		destPlayer->GetName().c_str(), destPlayer->GetGPID(),
		message.c_str());

	srcPlayer->GetServer()->SendPacket(responseSrc);
	destPlayer->GetServer()->SendPacket(responseDest);
}

void FriendsManager::HandleInviteFriendRequest(std::shared_ptr<SocialPlayer> player, SocialPacketServerInviteFriendRequest& packet) {
	if (nullptr == player)
		return;

	auto friendPlayer = player->FindFriend(packet.m_friendName);
	if (nullptr == friendPlayer)
		return;

	SocialPacketServerInviteConfirmFriendResponse sendPacket;
	sendPacket.m_gpid = friendPlayer->GetGPID();
	sendPacket.m_name = player->GetName();
	sendPacket.m_roomName = packet.m_roomName;
	sendPacket.m_roomNumber = packet.m_roomNumber;
	sendPacket.m_createdTime = packet.m_createdTime;
	sendPacket.m_maxPlayerCount = packet.m_maxPlayerCount;
	sendPacket.m_gameMode = packet.m_gameMode;
	sendPacket.m_ipAddress = player->GetServer()->GetServerIpAddress();
	sendPacket.m_port = player->GetServer()->GetServerPortNum();

	friendPlayer->GetServer()->SendPacket(sendPacket);
}
