#ifndef __LOGIN_MANAGER_HANDLER_H__
#define __LOGIN_MANAGER_HANDLER_H__

#include <memory>
#include "ManagementPacket.h"
#include "BaseServerHandler.h"

class LoginServer;

//class Player;

class LoginManagementHandler : public BaseServerHandler {
public:
	void Initialize();

	void HandleLoginPacket(Buffer& buffer, std::shared_ptr<Player> player, LoginPacketType type); //Client to server

	virtual void RegisterToServer() override {}
	virtual void HandlePacket(Buffer& buffer) override;
private:
	// Client to Login server
	void HandlePacketShowChannelRequest(ClientLoginPacketTypeShowChannelRequest& packet, std::shared_ptr<Player> player);
	void HandlePacketChannelInRequest(ClientLoginPacketTypeChannelInRequest& packet, std::shared_ptr<Player> player);

	// Social server to Game server
	void HandlePacketShowChannelResponse(LoginManagementPacketTypeShowChannelResponse& packet, std::shared_ptr<Player> player);
	void HandlePacketChannelInResponse(LoginManagementPacketTypeChannelInResponse& packet, std::shared_ptr<Player> player);

	LoginServer* m_loginServer;
};

#endif // __LOGIN_MANAGER_HANDLER_H__
