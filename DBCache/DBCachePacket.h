#ifndef __DBCACHE_PACKET_H__
#define __DBCACHE_PACKET_H__

#include "BasePacket.h"
#include "SaveData.h"
#include "LoadData.h"

enum DBCachePacketType : char {
	dbCachePacketTypeNone = 0,

	dbCacheSave, // (Table)table, (string)requestData
	dbCacheLoad, // (Table)table, (string)responseData

	dbCachePacketTypeSize,
};

class DBCachePacket : public BasePacket {
public:
	DBCachePacket(DBCachePacketType _dbCachePacketType) : BasePacket(BasePacketType::basePacketTypeDBCache), dbCachePacketType(_dbCachePacketType) {
		this->PacketTypeSerial(dbCachePacketType);
	}
	~DBCachePacket() {}
protected:
	DBCachePacketType dbCachePacketType = dbCachePacketTypeNone;
};

class DBCacheSave : public DBCachePacket {
public:
	DBCacheSave() : DBCachePacket(dbCacheSave) {}
	~DBCacheSave() {}

	virtual Buffer& Serialize() override {
		buffer << table;
		buffer << requestData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> table;
		_buf >> requestData;
	};

	db::Table table = db::tableNone;
	std::string requestData;
};

class DBCacheLoad : public DBCachePacket {
public:
	DBCacheLoad() : DBCachePacket(dbCacheLoad) {}
	~DBCacheLoad() {}

	virtual Buffer& Serialize() override {
		buffer << table;
		buffer << responseData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> table;
		_buf >> responseData;
	};

	db::Table table = db::tableNone;
	std::string responseData;
};

#endif