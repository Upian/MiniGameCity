#include "DBCache.h"

DBCache::DBCache() {}
DBCache::~DBCache() {}

void DBCache::HandleAcceptClient(SOCKET clientSocket) {

}

void DBCache::HandleDisconnectClient(SOCKET clientSocket) {

}

bool DBCache::Connect(const std::string& dbHost, const std::string& dbUser, const std::string& dbPass, const std::string& dbName, const int32& dbPort) {
	conn = mysql_init(nullptr);
	mysql_set_character_set(conn, "utf8");
	if (!mysql_real_connect(conn, dbHost.c_str(), dbUser.c_str(), dbPass.c_str(), dbName.c_str(), dbPort, nullptr, 0)) {
		printf("%s\n", mysql_error(conn)); // 접속 오류
		return false;
	}
	return true;
}

void DBCache::Close() {
	mysql_close(conn);
	printf("%s\n", mysql_error(conn)); // 닫기 오류
}

void DBCache::HandleBasePacket(BufferInfo* bufInfo) {
	if (nullptr == bufInfo)
		return;

	Util::LoggingInfo("DBCache.log", "Recv packet size: %d", bufInfo->buffer.Length());
	BasePacketType type = (BasePacketType)PacketTypeDeserial(bufInfo->buffer);
	switch (type) {
	case BasePacketType::basePacketTypeDBCache: {
		DBCachePacketType dbCacheType = (DBCachePacketType)PacketTypeDeserial(bufInfo->buffer);
		switch (dbCacheType) {
		case dbCacheSave: {
			
			
			
			break;
		}
		case dbCacheLoad: {
			
			
			
			break;
		}
		}
		break;
	}
	default: {
		Util::LoggingInfo("DBCache.log", "Recv wrong base packet ID : %d", type);
		break;
	}
	}
	printf("DBCache -- recv packet: %s\n", bufInfo->buffer);

}

