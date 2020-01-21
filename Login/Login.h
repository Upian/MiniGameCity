#pragma once
#ifndef __LOGIN_LOGIN_H__
#define __LOGIN_LOGIN_H__

#include "BaseServer.h"
#include "ServerCommon.h"

class LoginServer : public BaseServer<LoginServer> {
public:
	virtual void HandleBasePacket(BufferInfo* buf);

private:

};




#endif // !__LOGIN_LOGIN_SERVER_H__
