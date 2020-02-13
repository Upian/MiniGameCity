#ifndef __DBCACHE_PACKET_H__
#define __DBCACHE_PACKET_H__

#include "BasePacket.h"

enum DBCachePacketType : char {
	dbCachePacketTypeNone = 0,

	// management server <-> db 
	managementDBCachePacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8), (unsigned int)GPID
	managementDBCachePacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	managementDBCachePacketTypeLogoutRequest, //(unsigned int)GPID
	managementDBCachePacketTypeSignupResponse, //(bool)flag
	managementDBCachePacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)nick(4~8),
	managementDBCachePacketTypeDeleteResponse, //(bool)flag
	managementDBCachePacketTypeDeleteRequest, //(unsigned int)GPID

	dbCachePacketTypeSize,
};

/*

		  login <-> management

*/

class DBCachePacket : public BasePacket {
public:
	DBCachePacket(DBCachePacketType _dbCachePacketType) : BasePacket(BasePacketType::managementPacketTypeDBCache), dbCachePacketType(_dbCachePacketType) {
		this->PacketTypeSerial(dbCachePacketType);
	}
	~DBCachePacket() {}
protected:
	DBCachePacketType dbCachePacketType = dbCachePacketTypeNone;
};

class ManagementDBCachePacketTypeLoginResponse : public DBCachePacket {
public:
	ManagementDBCachePacketTypeLoginResponse() : DBCachePacket(managementDBCachePacketTypeLoginResponse) {}
	~ManagementDBCachePacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << userNick;
		buffer << GPID;
		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> userNick;
		_buf >> GPID;
	};

	bool flag = true;
	std::string userNick{};
	uint32 GPID = 0;
};

class ManagementDBCachePacketTypeLoginRequest : public DBCachePacket {
public:
	ManagementDBCachePacketTypeLoginRequest() : DBCachePacket(managementDBCachePacketTypeLoginRequest) {}
	~ManagementDBCachePacketTypeLoginRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> userId;
		_buf >> userPw;
	};

	std::string userId{};
	std::string userPw{};
};

class ManagementDBCachePacketTypeLogoutRequest : public DBCachePacket {
public:
	ManagementDBCachePacketTypeLogoutRequest() : DBCachePacket(managementDBCachePacketTypeLogoutRequest) {}
	~ManagementDBCachePacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {
		buffer << GPID;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> GPID;
	}
	uint32 GPID = 0;
};

class ManagementDBCachePacketTypeSignupResponse : public DBCachePacket {
public:
	ManagementDBCachePacketTypeSignupResponse() : DBCachePacket(managementDBCachePacketTypeSignupResponse) {}
	~ManagementDBCachePacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class ManagementDBCachePacketTypeSignupRequest : public DBCachePacket {
public:
	ManagementDBCachePacketTypeSignupRequest() : DBCachePacket(managementDBCachePacketTypeSignupRequest) {}
	~ManagementDBCachePacketTypeSignupRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;
		buffer << userNick;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> userId;
		_buf >> userPw;
		_buf >> userNick;
	}

	std::string userId;
	std::string userPw;
	std::string userNick;
};

class ManagementDBCachePacketTypeDeleteResponse : public DBCachePacket {
public:
	ManagementDBCachePacketTypeDeleteResponse() : DBCachePacket(managementDBCachePacketTypeDeleteResponse) {}
	~ManagementDBCachePacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class ManagementDBCachePacketTypeDeleteRequest : public DBCachePacket {
public:
	ManagementDBCachePacketTypeDeleteRequest() : DBCachePacket(managementDBCachePacketTypeDeleteRequest) {}
	~ManagementDBCachePacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {
		buffer << GPID;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> GPID;
	}

	uint32 GPID = 0;
};

#endif