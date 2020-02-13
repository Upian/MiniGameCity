#include "FriendsManager.h"
#include "SocialPlayer.h"
#include "ClntServer.h"
#include "SocialServerPacket.h"
#include "Log.h"
void FriendsManager::HandleAddFriendRequest(std::shared_ptr<SocialPlayer> srcPlayer, std::shared_ptr<SocialPlayer> destPlayer) {
	if (nullptr == srcPlayer)
		return;
	
	if (nullptr == destPlayer) {
		//#DatabaseLoad
		//Handle database
		return;
	}

	SocialPacketServerAddFriendResponse packet;
	packet.m_gpid = srcPlayer->GetGPID();
	packet.m_errorType = destPlayer->AddFriendRequest(srcPlayer);
	if (srcPlayer == destPlayer)
		packet.m_errorType = ErrorTypeAddFriend::samePlayer;

	if (ErrorTypeAddFriend::none != packet.m_errorType) //Fail
		packet.m_success = false;
	else 
		packet.m_success = true;

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
		packet.m_names.emplace_back(p->GetName());
	}

	pplayer->GetServer()->SendPacket(packet);
}

void FriendsManager::HandleAcceptFriendReequest(std::shared_ptr<SocialPlayer> src, std::shared_ptr<SocialPlayer> dest) {
	if (false == dest->IsExistFriendRequestList(src))
		return;
	if (src == dest)
		return;

	SocialPacketServerAcceptFriendResponse packet;
	packet.m_gpid = src->GetGPID();
	packet.m_errorCode = ErrorTypeAcceptFriend::none;
	if (false == src->AddFriendList(src))
		packet.m_errorCode = ErrorTypeAcceptFriend::srcFriendListIsFull;
	if (false == dest->AddFriendList(dest))
		packet.m_errorCode = ErrorTypeAcceptFriend::destFriendListIsFull;

	if (ErrorTypeAcceptFriend::none != packet.m_errorCode) { //둘 중 하나라도 친구 추가 실패 하면
		src->DeleteFriendList(dest);
		dest->DeleteFriendList(src);
	}
	src->GetServer()->SendPacket(packet);
}
