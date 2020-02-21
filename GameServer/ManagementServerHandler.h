#pragma once
#ifndef __GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__
#define __GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__

#include "BaseServerHandler.h"

class GameServer;

class ManagementServerHandler : public BaseServerHandler {
public:
	void Initialize();

	void RegisterToServer();
	virtual void HandlePacket(Buffer& buffer) override;

	void HandleTrasferForInviteRequest(std::string ip, int port, std::shared_ptr<Player> pPlayer);
	
	
	void HandleTransferChannelResponse(bool, std::string, std::shared_ptr<Player>);

private:
	void HandlePreLoadPacket(GPID, SessionID);

	GameServer* m_gameServer = nullptr;
};
#endif // !__GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__
