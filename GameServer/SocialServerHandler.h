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

//packet
struct SocialPacketServerAddFriendResponse;

class SocialServerHandler : public BaseServerHandler{
public:
	void Initialize();

	void HandleSocialPacket(Buffer& buffer, std::shared_ptr<Player> player); //Client to server

	void UpdatePlayerInfoAtLogin(std::shared_ptr<Player>);
	void UpdatePlayerInfoAtLogout(std::shared_ptr<Player>);

	virtual void RegisterToServer() override {}
	virtual void HandlePacket(Buffer& buffer) override;
private:
	//Client to Game server
	void HandlePacketChatNormalRequest(SocialGamePacketChatNormalRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketAddFriendRequest(SocialGamePacketAddFriendRequest& packet, std::shared_ptr<Player> player);

	//Social server to Game server
	void HandlePacketAddFriendResponse(SocialPacketServerAddFriendResponse& packet, std::shared_ptr<Player> pplayer);


	GameServer* m_gameServer;
};

#endif // !__GAMESERVER_SOCIAL_SERVER_HANDLER_H__
