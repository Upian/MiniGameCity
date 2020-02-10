#pragma once
#ifndef __GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__
#define __GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__

#include "BaseServerHandler.h"

class ManagementServerHandler : public BaseServerHandler {
public:
	virtual void RegisterToServer() override {}
	virtual void HandlePacket(Buffer& buffer) override {}
private:

};
#endif // !__GAMESERVER_MANAGEMENT_SERVER_HANDLER_H__
