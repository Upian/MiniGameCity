#ifndef __DB_CACHE_H__
#define __DB_CACHE_H__

#include "ServerCommon.h"
#include "BaseServer.h"
#include "DatabaseAPI.h"
#include "DBCachePacket.h"

class DBCache : public BaseServer<DBCache> {
public:
	

private:
	MAKE_SERVER(DBCache)
	void HandlePacketLogin(BufferInfo* bufInfo);

};

#endif // __DB_CACHE_H__