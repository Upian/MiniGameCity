#pragma once
#ifndef __GAMESERVER_SOCIAL_SERVER_HANDLER_H__
#define __GAMESERVER_SOCIAL_SERVER_HANDLER_H__
#include <memory>
#include "SocialServerPacket.h"
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

	virtual void RegisterToServer() override;
	virtual void HandlePacket(Buffer& buffer) override;
private:
	//Client to Game server
	void HandlePacketChatNormalRequest(SocialGamePacketChatNormalRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketChatFriendRequest(SocialGamePacketChatFriendRequest&, std::shared_ptr<Player>);
	void HandlePacketChatGuildRequest();
	void HandlePacketAddFriendRequest(SocialGamePacketAddFriendRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketConfirmFriendRequest(std::shared_ptr<Player> player);
	void HandlePacketAcceptFriendRequest(SocialGamePacketAcceptFriendRequest&, std::shared_ptr<Player>);
	void HandlePacketFriendListRequest(std::shared_ptr<Player>);
	void HandlePacketDeleteFriendRequest(SocialGamePacketDeleteFriendRequest&, std::shared_ptr<Player>);
	void HandlePacketInviteFriendRequest(SocialGamePacketInviteFriendRequest&, std::shared_ptr<Player>);

	//Social server to Game server
	void HandlePacketAddFriendResponse(SocialPacketServerAddFriendResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketDeleteFriendResponse(SocialPacketServerDeleteFriendResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketConfirmFriendResponse(SocialPacketServerConfirmFriendResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketAcceptFriendResponse(SocialPacketServerAcceptFriendResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketFriendListResponse(SocialPacketServerFriendListResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketChatFriendResponse(SocialPacketServerChatFriendResponse& packet, std::shared_ptr<Player> pplayer);
	void HandlePacketInviteConfirmRequest(SocialPacketServerInviteConfirmFriendRequest& packet, std::shared_ptr<Player> pplayer);

	std::shared_ptr<Player> GetPlayer(GPID gpid);
	GameServer* m_gameServer = nullptr;
};

#endif // !__GAMESERVER_SOCIAL_SERVER_HANDLER_H__
