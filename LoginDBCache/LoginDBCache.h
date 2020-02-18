#ifndef __DB_CACHE_H__
#define __DB_CACHE_H__

#include "ServerCommon.h"
#include "BaseServer.h"
#include "DatabaseAPI.h"
#include "LoginDBCachePacket.h"

class LoginDBCache : public BaseServer<LoginDBCache> {
public:


private:
	MAKE_SERVER(LoginDBCache)
		void HandlePacketLogin(BufferInfo* bufInfo);

};

#endif // __DB_CACHE_H__