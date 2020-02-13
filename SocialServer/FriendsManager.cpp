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

	destPlayer->AddFriendRequest(srcPlayer);

	SocialPacketServerAddFriendResponse packet;
	packet.m_gpid = srcPlayer->GetGPID();
	packet.m_success = true;
	srcPlayer->GetServer()->SendPacket(packet);
	Util::LoggingDebug("", "Send packet to game server");
}
