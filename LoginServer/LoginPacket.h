#ifndef LOGIN_PACKET_H
#define LOGIN_PACKET_H
#include "BasePacket.h"
#include "ManagementPacket.h"

#define CHANNEL_SIZE 4

enum LoginPacketType : char {
	loginPacketTypeNone = 0,

	// client <-> login server
	clientLoginPacketTypeLoginResponse, //(bool)flag(1), (string)userNick(4~8)
	clientLoginPacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	clientLoginPacketTypeLogoutRequest, //nothing
	clientLoginPacketTypeSignupResponse, //(bool)flag
	clientLoginPacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)userNick(4~8),
	clientLoginPacketTypeDeleteResponse, //(bool)flag
	clientLoginPacketTypeDeleteRequest, //nothing
	clientLoginPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	clientLoginPacketTypeShowChannelRequest, //nothing.
	clientLoginPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	clientLoginPacketTypeChannelInRequest, //(string)channelName

	loginPacketTypeSize,

};

/*

		  client <-> login

*/

class LoginPacket : public BasePacket {
public:
	LoginPacket(LoginPacketType _loginPacketType) : BasePacket(BasePacketType::basePacketTypeLogin), loginPacketType(_loginPacketType) {
		this->PacketTypeSerial(loginPacketType);
	}
	~LoginPacket() {}
protected:
	LoginPacketType loginPacketType = loginPacketTypeNone;
};

class ClientLoginPacketTypeLoginResponse : public LoginPacket {
public:
	ClientLoginPacketTypeLoginResponse() : LoginPacket(clientLoginPacketTypeLoginResponse) {}
	~ClientLoginPacketTypeLoginResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;
		buffer << userNick;
		return buffer;
	};
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> userNick;
	};

	bool flag = true;
	std::string userNick{};
};

class ClientLoginPacketTypeLoginRequest : public LoginPacket {
public:
	ClientLoginPacketTypeLoginRequest() : LoginPacket(clientLoginPacketTypeLoginRequest) {}
	~ClientLoginPacketTypeLoginRequest() {}

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

class ClientLoginPacketTypeLogoutRequest : public LoginPacket {
public:
	ClientLoginPacketTypeLogoutRequest() : LoginPacket(clientLoginPacketTypeLogoutRequest) {}
	~ClientLoginPacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {
		buffer << serialNumber;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> serialNumber;
	}
	uint32 serialNumber = 0;
};

class ClientLoginPacketTypeSignupResponse : public LoginPacket {
public:
	ClientLoginPacketTypeSignupResponse() : LoginPacket(clientLoginPacketTypeSignupResponse) {}
	~ClientLoginPacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		buffer >> flag;
	}

	bool flag = true;
};

class ClientLoginPacketTypeSignupRequest : public LoginPacket {
public:
	ClientLoginPacketTypeSignupRequest() : LoginPacket(clientLoginPacketTypeSignupRequest) {}
	~ClientLoginPacketTypeSignupRequest() {}

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

class ClientLoginPacketTypeDeleteResponse : public LoginPacket {
public:
	ClientLoginPacketTypeDeleteResponse() : LoginPacket(clientLoginPacketTypeDeleteResponse) {}
	~ClientLoginPacketTypeDeleteResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
	}

	bool flag = true;
};

class ClientLoginPacketTypeDeleteRequest : public LoginPacket {
public:
	ClientLoginPacketTypeDeleteRequest() : LoginPacket(clientLoginPacketTypeDeleteRequest) {}
	~ClientLoginPacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {
		buffer << serialNumber;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> serialNumber;
	}
	uint32 serialNumber = 0;
};

class ClientLoginPacketTypeShowChannelResponse : public LoginPacket {
public:
	ClientLoginPacketTypeShowChannelResponse() : LoginPacket(clientLoginPacketTypeShowChannelResponse) {}
	~ClientLoginPacketTypeShowChannelResponse() {}

	virtual Buffer& Serialize() override {
		for (int i = 0; i < CHANNEL_SIZE; ++i) {
			buffer << channel[i].channelName;
			buffer << channel[i].numberOfPeople;
			buffer << channel[i].limitOfPeople;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		for (int i = 0; i < Util::GetConfigToInt("LoginServer.ini", "Definition", "ChannelSize"); ++i) {
			_buf >> channel[i].channelName;
			_buf >> channel[i].numberOfPeople;
			_buf >> channel[i].limitOfPeople;
		}
	}

	Channel channel[CHANNEL_SIZE]{}; // 개수 바뀔 수 있음.
};

class ClientLoginPacketTypeShowChannelRequest : public LoginPacket {
public:
	ClientLoginPacketTypeShowChannelRequest() : LoginPacket(clientLoginPacketTypeShowChannelRequest) {}
	~ClientLoginPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class ClientLoginPacketTypeChannelInResponse : public LoginPacket {
public:
	ClientLoginPacketTypeChannelInResponse() : LoginPacket(clientLoginPacketTypeChannelInResponse) {}
	~ClientLoginPacketTypeChannelInResponse() {}

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

class ClientLoginPacketTypeChannelInRequest : public LoginPacket {
public:
	ClientLoginPacketTypeChannelInRequest() : LoginPacket(clientLoginPacketTypeChannelInRequest) {}
	~ClientLoginPacketTypeChannelInRequest() {}

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