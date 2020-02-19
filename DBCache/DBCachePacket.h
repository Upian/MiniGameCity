#ifndef __DBCACHE_PACKET_H__
#define __DBCACHE_PACKET_H__

#include "BasePacket.h"
#include "SaveData.h"
#include "LoadData.h"

enum DBCachePacketType : char {
	dbCachePacketTypeNone = 0,

	dbCacheSave, // (Command)command, (Table)table, (string)requestData
	dbCacheLoad, // (Command)command, (Table)table, (string)responseData

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
		buffer << command;
		buffer << table;
		buffer << requestData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> command;
		_buf >> table;
		_buf >> requestData;
	};

	Database::Command command = Database::commandNone;
	Database::Table table = Database::tableNone;
	std::string requestData;
};

class DBCacheLoad : public DBCachePacket {
public:
	DBCacheLoad() : DBCachePacket(dbCacheLoad) {}
	~DBCacheLoad() {}

	virtual Buffer& Serialize() override {

		buffer << responseData;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> responseData;
	};

	Database::Command command = Database::commandNone;
	Database::Table table = Database::tableNone;
	std::string responseData;
};

#endif