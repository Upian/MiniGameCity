#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__
#include "BasePacket.h"

// must go to ini file 
#define CHANNEL_SIZE 4
#define ID_SIZE 8
#define PW_SIZE 16
#define NICK_SIZE 24

enum ManagementPacketType : char {
	managementPacketTypeNone = 0,


	managementPacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8)
	managementPacketTypeLoginRequest, //(string)accountId(4~8), (string)accountPw(8~16)  
	managementPacketTypeLogoutRequest, //nothing
	managementPacketTypeSignupResponse, //(bool)flag
	managementPacketTypeSignupRequest, //(string)accountId(4~8), (string)accountPw(8~16), (string)nick(4~8),
	managementPacketTypeDeleteResponse, //(bool)flag
	managementPacketTypeDeleteRequest, //nothing
	managementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	managementPacketTypeShowChannelRequest, //nothing.
	managementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	managementPacketTypeChannelInRequest, //(string)channelName
	
	


	
	
	managementPacketTypeSize,
};








struct Channel {
	std::string channelName;
	int32 numberOfPeople = 0;
	int32 limitOfPeople = 0;
};

class ManagementPacket : public BasePacket {
public:
	ManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::serverLoginPacketTypeManagement), managementPacketType(_managementPacketType) {
		this->PacketTypeSerial(managementPacketType);
	}
	~ManagementPacket() {}
protected:
	ManagementPacketType managementPacketType = managementPacketTypeNone;
};

class ManagementPacketTypeLoginResponse : public ManagementPacket {
public:
	ManagementPacketTypeLoginResponse() : ManagementPacket(managementPacketTypeLoginResponse) {}
	~ManagementPacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {

	}
	virtual void Deserialize(Buffer& _buf) override {

	}

};





class LoginPacketTypeLoginResponse : public LoginPacket {
public:
	LoginPacketTypeLoginResponse() : LoginPacket(loginPacketTypeLoginResponse) {}
	~LoginPacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << nick;
		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> nick;
	};

	bool flag = true;
	std::string nick{};
};

class LoginPacketTypeLoginRequest : public LoginPacket {
public:
	LoginPacketTypeLoginRequest() : LoginPacket(loginPacketTypeLoginRequest) {}
	~LoginPacketTypeLoginRequest() {}

	virtual Buffer& Serialize() override {
		buffer << accountId;
		buffer << accountPw;

		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> accountId;
		_buf >> accountPw;
	};

	std::string accountId{};
	std::string accountPw{};
};

class LoginPacketTypeLogoutRequest : public LoginPacket {
public:
	LoginPacketTypeLogoutRequest() : LoginPacket(loginPacketTypeLogoutRequest) {}
	~LoginPacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}

};

class LoginPacketTypeSignupResponse : public LoginPacket {
public:
	LoginPacketTypeSignupResponse() : LoginPacket(loginPacketTypeSignupResponse) {}
	~LoginPacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		buffer >> flag;
	}

	bool flag = true;
};

class LoginPacketTypeSignupRequest : public LoginPacket {
public:
	LoginPacketTypeSignupRequest() : LoginPacket(loginPacketTypeSignupRequest) {}
	~LoginPacketTypeSignupRequest() {}

	virtual Buffer& Serialize() override {
		buffer << accountId;
		buffer << accountPw;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> accountId;
		_buf >> accountPw;
		_buf >> nick;
	}

	std::string accountId;
	std::string accountPw;
	std::string nick;
};

class LoginPacketTypeDeleteResponse : public LoginPacket {
public:
	LoginPacketTypeDeleteResponse() : LoginPacket(loginPacketTypeDeleteResponse) {}
	~LoginPacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class LoginPacketTypeDeleteRequest : public LoginPacket {
public:
	LoginPacketTypeDeleteRequest() : LoginPacket(loginPacketTypeDeleteRequest) {}
	~LoginPacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class LoginPacketTypeShowChannelResponse : public LoginPacket {
public:
	LoginPacketTypeShowChannelResponse() : LoginPacket(loginPacketTypeShowChannelResponse) {}
	~LoginPacketTypeShowChannelResponse() {}

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

class LoginPacketTypeShowChannelRequest : public LoginPacket {
public:
	LoginPacketTypeShowChannelRequest() : LoginPacket(loginPacketTypeShowChannelRequest) {}
	~LoginPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class LoginPacketTypeChannelInResponse : public LoginPacket {
public:
	LoginPacketTypeChannelInResponse() : LoginPacket(loginPacketTypeChannelInResponse) {}
	~LoginPacketTypeChannelInResponse() {}

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

class LoginPacketTypeChannelInRequest : public LoginPacket {
public:
	LoginPacketTypeChannelInRequest() : LoginPacket(loginPacketTypeChannelInRequest) {}
	~LoginPacketTypeChannelInRequest() {}

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