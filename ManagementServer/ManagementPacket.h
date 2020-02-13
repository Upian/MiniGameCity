#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__
#include "DBCachePacket.h"

// must go to ini file 
#define CHANNEL_SIZE 4 // test

enum ManagementPacketType : char {
	managementPacketTypeNone = 0,

	// login server <-> management sserver
	loginManagementPacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8), (unsigned int)GPID
	loginManagementPacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	loginManagementPacketTypeLogoutRequest, //(unsigned int)GPID
	loginManagementPacketTypeSignupResponse, //(bool)flag
	loginManagementPacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)nick(4~8),
	loginManagementPacketTypeDeleteResponse, //(bool)flag
	loginManagementPacketTypeDeleteRequest, //(unsigned int)GPID
	loginManagementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	loginManagementPacketTypeShowChannelRequest, //nothing.
	loginManagementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	loginManagementPacketTypeChannelInRequest, //(string)channelName

	managementPacketTypeSize,
};

struct Channel {
	std::string channelName;
	int32 numberOfPeople = 0;
	int32 limitOfPeople = 0;
};

/*

		  login <-> management

*/

class ManagementPacket : public BasePacket {
public:
	ManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::loginPacketTypeManagement), managementPacketType(_managementPacketType) {
		this->PacketTypeSerial(managementPacketType);
	}
	~ManagementPacket() {}
protected:
	ManagementPacketType managementPacketType = managementPacketTypeNone;
};

class LoginManagementPacketTypeLoginResponse : public ManagementPacket {
public:
	LoginManagementPacketTypeLoginResponse() : ManagementPacket(loginManagementPacketTypeLoginResponse) {}
	~LoginManagementPacketTypeLoginResponse() {}

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

class LoginManagementPacketTypeLoginRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeLoginRequest() : ManagementPacket(loginManagementPacketTypeLoginRequest) {}
	~LoginManagementPacketTypeLoginRequest() {}

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

class LoginManagementPacketTypeLogoutRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeLogoutRequest() : ManagementPacket(loginManagementPacketTypeLogoutRequest) {}
	~LoginManagementPacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {
		buffer << GPID;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> GPID;
	}
	uint32 GPID = 0;
};

class LoginManagementPacketTypeSignupResponse : public ManagementPacket {
public:
	LoginManagementPacketTypeSignupResponse() : ManagementPacket(loginManagementPacketTypeSignupResponse) {}
	~LoginManagementPacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class LoginManagementPacketTypeSignupRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeSignupRequest() : ManagementPacket(loginManagementPacketTypeSignupRequest) {}
	~LoginManagementPacketTypeSignupRequest() {}

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

class LoginManagementPacketTypeDeleteResponse : public ManagementPacket {
public:
	LoginManagementPacketTypeDeleteResponse() : ManagementPacket(loginManagementPacketTypeDeleteResponse) {}
	~LoginManagementPacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class LoginManagementPacketTypeDeleteRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeDeleteRequest() : ManagementPacket(loginManagementPacketTypeDeleteRequest) {}
	~LoginManagementPacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {
		buffer << GPID;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> GPID;
	}

	uint32 GPID = 0;
};

class LoginManagementPacketTypeShowChannelResponse : public ManagementPacket {
public:
	LoginManagementPacketTypeShowChannelResponse() : ManagementPacket(loginManagementPacketTypeShowChannelResponse) {}
	~LoginManagementPacketTypeShowChannelResponse() {}

	virtual Buffer& Serialize() override {
		for (int i = 0; i < CHANNEL_SIZE; ++i) {
			buffer << channel[i].channelName;
			buffer << channel[i].numberOfPeople;
			buffer << channel[i].limitOfPeople;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		for (int i = 0; i < CHANNEL_SIZE; ++i) {
			_buf >> channel[i].channelName;
			_buf >> channel[i].numberOfPeople;
			_buf >> channel[i].limitOfPeople;
		}
	}

	Channel channel[CHANNEL_SIZE]{}; // 개수 바뀔 수 있음.
};

class LoginManagementPacketTypeShowChannelRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeShowChannelRequest() : ManagementPacket(loginManagementPacketTypeShowChannelRequest) {}
	~LoginManagementPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class LoginManagementPacketTypeChannelInResponse : public ManagementPacket {
public:
	LoginManagementPacketTypeChannelInResponse() : ManagementPacket(loginManagementPacketTypeChannelInResponse) {}
	~LoginManagementPacketTypeChannelInResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << ip;
		buffer << port;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> ip;
		_buf >> port;
	}

	bool flag = true;
	std::string ip;
	int16 port = 0;
};

class LoginManagementPacketTypeChannelInRequest : public ManagementPacket {
public:
	LoginManagementPacketTypeChannelInRequest() : ManagementPacket(loginManagementPacketTypeChannelInRequest) {}
	~LoginManagementPacketTypeChannelInRequest() {}

	virtual Buffer& Serialize() override {
		buffer << channelName;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelName;
	}

	std::string channelName;
};

#endif