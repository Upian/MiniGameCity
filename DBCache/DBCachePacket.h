#ifndef __DBCACHE_PACKET_H__
#define __DBCACHE_PACKET_H__

#include "BasePacket.h"

enum DBCachePacketType : char {
	dbCachePacketTypeNone = 0,

	dbCacheSave, // (string)data
	dbCacheLoad, // (string)data

	dbCachePacketTypeSize,
};

/*

		  login server <-> db cache

*/

class DBCachePacket : public BasePacket {
public:
	DBCachePacket(DBCachePacketType _dbCachePacketType) : BasePacket(BasePacketType::basePacketTypeDBCache), dbCachePacketType(_dbCachePacketType) {
		this->PacketTypeSerial(dbCachePacketType);
	}
	~DBCachePacket() {}
protected:
	DBCachePacketType dbCachePacketType = dbCachePacketTypeNone;
};

class LoginDBCachePacketTypeLoginResponse : public DBCachePacket {
public:
	LoginDBCachePacketTypeLoginResponse() : DBCachePacket(dbCacheSave) {}
	~LoginDBCachePacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << requestData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> requestData;
	};

	std::string requestData;
};
class LoginDBCachePacketTypeLoginResponse : public DBCachePacket {
public:
	LoginDBCachePacketTypeLoginResponse() : DBCachePacket(dbCacheLoad) {}
	~LoginDBCachePacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << responseData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> responseData;
	};

	std::string responseData;
};

#endif