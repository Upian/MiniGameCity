#ifndef __LOGIN_DBCACHE_PACKET_H__
#define __LOGIN_DBCACHE_PACKET_H__

#include "LoginPacket.h"

enum DBCachePacketType : char {
	dbCachePacketTypeNone = 0,

	// login server <-> db server
	loginDBCachePacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8), (unsigned int)gpid, (string)userId
	loginDBCachePacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	loginDBCachePacketTypeLogoutRequest, //(unsigned int)gpid
	loginDBCachePacketTypeSignupResponse, //(bool)flag, (unsigned int)gpid, (string)userId
	loginDBCachePacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)nick(4~8),
	loginDBCachePacketTypeDeleteResponse, //(bool)flag, (unsigned int)gpid
	loginDBCachePacketTypeDeleteRequest, //(unsigned int)gpid

	dbCachePacketTypeSize,
};

/*

		  login server <-> db cache

*/

class DBCachePacket : public BasePacket {
public:
	DBCachePacket(DBCachePacketType _dbCachePacketType) : BasePacket(BasePacketType::loginPacketTypeDBCache), dbCachePacketType(_dbCachePacketType) {
		this->PacketTypeSerial(dbCachePacketType);
	}
	~DBCachePacket() {}
protected:
	DBCachePacketType dbCachePacketType = dbCachePacketTypeNone;
};

class LoginDBCachePacketTypeLoginResponse : public DBCachePacket {
public:
	LoginDBCachePacketTypeLoginResponse() : DBCachePacket(loginDBCachePacketTypeLoginResponse) {}
	~LoginDBCachePacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << userNick;
		buffer << gpid;
		buffer << userId;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> userNick;
		_buf >> gpid;
		_buf >> userId;
	};

	bool flag = true;
	std::string userNick;
	uint32 gpid = 0;
	std::string userId;
};

class LoginDBCachePacketTypeLoginRequest : public DBCachePacket {
public:
	LoginDBCachePacketTypeLoginRequest() : DBCachePacket(loginDBCachePacketTypeLoginRequest) {}
	~LoginDBCachePacketTypeLoginRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> userId;
		_buf >> userPw;
	};

	std::string userId;
	std::string userPw;
};

class LoginDBCachePacketTypeLogoutRequest : public DBCachePacket {
public:
	LoginDBCachePacketTypeLogoutRequest() : DBCachePacket(loginDBCachePacketTypeLogoutRequest) {}
	~LoginDBCachePacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> gpid;
	}
	uint32 gpid = 0;
};

class LoginDBCachePacketTypeSignupResponse : public DBCachePacket {
public:
	LoginDBCachePacketTypeSignupResponse() : DBCachePacket(loginDBCachePacketTypeSignupResponse) {}
	~LoginDBCachePacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << gpid;
		buffer << userId;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> gpid;
		_buf >> userId;
	}

	bool flag = true;
	uint32 gpid = 0;
	std::string userId;
};

class LoginDBCachePacketTypeSignupRequest : public DBCachePacket {
public:
	LoginDBCachePacketTypeSignupRequest() : DBCachePacket(loginDBCachePacketTypeSignupRequest) {}
	~LoginDBCachePacketTypeSignupRequest() {}

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

class LoginDBCachePacketTypeDeleteResponse : public DBCachePacket {
public:
	LoginDBCachePacketTypeDeleteResponse() : DBCachePacket(loginDBCachePacketTypeDeleteResponse) {}
	~LoginDBCachePacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> gpid;
	}

	bool flag = true;
	uint32 gpid = 0;
};

class LoginDBCachePacketTypeDeleteRequest : public DBCachePacket {
public:
	LoginDBCachePacketTypeDeleteRequest() : DBCachePacket(loginDBCachePacketTypeDeleteRequest) {}
	~LoginDBCachePacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> gpid;
	}

	uint32 gpid = 0;
};

#endif