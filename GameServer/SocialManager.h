#pragma once
#ifndef __GAMESERVER_SOCIAL_MANAGER_H__
#define __GAMESERVER_SOCIAL_MANAGER_H__
#include <memory>
#include "SocialPacket.h"

class GameServer;
class SocialServer;
class Player;
class Buffer;

class SocialManager {
public:
	void Initialize();

	void HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player);

	

private:
	void HandlePacketChatNormal(SocialPacketChatNormalRequest& packet, std::shared_ptr<Player> player);


	GameServer* m_gameServer;
	SocialServer* m_socialServer;
};

#endif // !__GAMESERVER_SOCIAL_MANAGER_H__
