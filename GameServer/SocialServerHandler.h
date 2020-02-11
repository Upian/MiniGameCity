#pragma once
#ifndef __GAMESERVER_SOCIAL_SERVER_HANDLER_H__
#define __GAMESERVER_SOCIAL_SERVER_HANDLER_H__
#include <memory>
#include "SocialGamePacket.h"
#include "BaseServerHandler.h"

class GameServer;
class SocialServer;
class Player;
class Buffer;

class SocialServerHandler : public BaseServerHandler{
public:
	void Initialize();

	void HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player); //Client to server

	virtual void RegisterToServer() override {}
	virtual void HandlePacket(Buffer& buffer) override {}
private:
	void HandlePacketChatNormal(SocialGamePacketChatNormalRequest& packet, std::shared_ptr<Player> player);

	GameServer* m_gameServer;
};

#endif // !__GAMESERVER_SOCIAL_SERVER_HANDLER_H__
