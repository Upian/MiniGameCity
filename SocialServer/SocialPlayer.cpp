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

ErrorTypeAddFriend SocialPlayer::AddFriendRequest(std::shared_ptr<SocialPlayer> pplayer) {
	if (nullptr == pplayer)
		return ErrorTypeAddFriend::notExistPlayer;
	if (m_maxFriendListSize <= m_friendList.size())
		return ErrorTypeAddFriend::destFriendListIsFull;
	if (m_maxFriendRequestSize <= m_friendRequestList.size())
		return ErrorTypeAddFriend::destFriendRequestListIsFull;


	m_friendRequestList.push_back(pplayer);
	return ErrorTypeAddFriend::none;
}


