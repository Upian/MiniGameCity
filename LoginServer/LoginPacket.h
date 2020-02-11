#ifndef LOGIN_PACKET_H
#define LOGIN_PACKET_H
#include "BasePacket.h"

#define CHANNEL_SIZE 4
#define ID_SIZE 8
#define PW_SIZE 16
#define NICK_SIZE 24

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

	// login server <-> management sserver
	loginManagementPacketTypeLoginResponse, //(bool)flag(1), (string)nick(4~8), (unsigned int)serialNumber
	loginManagementPacketTypeLoginRequest, //(string)userId(4~8), (string)userPw(8~16)  
	loginManagementPacketTypeLogoutRequest, //(unsigned int)serialNumber
	loginManagementPacketTypeSignupResponse, //(bool)flag
	loginManagementPacketTypeSignupRequest, //(string)userId(4~8), (string)userPw(8~16), (string)nick(4~8),
	loginManagementPacketTypeDeleteResponse, //(bool)flag
	loginManagementPacketTypeDeleteRequest, //(unsigned int)serialNumber
	loginManagementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople)
	loginManagementPacketTypeShowChannelRequest, //nothing.
	loginManagementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	loginManagementPacketTypeChannelInRequest, //(string)channelName

	loginPacketTypeSize,

};

struct Channel {
	std::string channelName;
	int32 numberOfPeople = 0;
	int32 limitOfPeople = 0;
};

/*

		  client <-> login

*/

class ClientLoginPacket : public BasePacket {
public:
	ClientLoginPacket(LoginPacketType _loginPacketType) : BasePacket(BasePacketType::basePacketTypeLogin), loginPacketType(_loginPacketType) {
		this->PacketTypeSerial(loginPacketType);
	}
	~ClientLoginPacket() {}
protected:
	LoginPacketType loginPacketType = loginPacketTypeNone;
};

class ClientLoginPacketTypeLoginResponse : public ClientLoginPacket {
public:
	ClientLoginPacketTypeLoginResponse() : ClientLoginPacket(clientLoginPacketTypeLoginResponse) {}
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

class ClientLoginPacketTypeLoginRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeLoginRequest() : ClientLoginPacket(clientLoginPacketTypeLoginRequest) {}
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

class ClientLoginPacketTypeLogoutRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeLogoutRequest() : ClientLoginPacket(clientLoginPacketTypeLogoutRequest) {}
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

class ClientLoginPacketTypeSignupResponse : public ClientLoginPacket {
public:
	ClientLoginPacketTypeSignupResponse() : ClientLoginPacket(clientLoginPacketTypeSignupResponse) {}
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

class ClientLoginPacketTypeSignupRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeSignupRequest() : ClientLoginPacket(clientLoginPacketTypeSignupRequest) {}
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

class ClientLoginPacketTypeDeleteResponse : public ClientLoginPacket {
public:
	ClientLoginPacketTypeDeleteResponse() : ClientLoginPacket(clientLoginPacketTypeDeleteResponse) {}
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

class ClientLoginPacketTypeDeleteRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeDeleteRequest() : ClientLoginPacket(clientLoginPacketTypeDeleteRequest) {}
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

class ClientLoginPacketTypeShowChannelResponse : public ClientLoginPacket {
public:
	ClientLoginPacketTypeShowChannelResponse() : ClientLoginPacket(clientLoginPacketTypeShowChannelResponse) {}
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
		for (int i = 0; i < CHANNEL_SIZE; ++i) {
			_buf >> channel[i].channelName;
			_buf >> channel[i].numberOfPeople;
			_buf >> channel[i].limitOfPeople;
		}
	}

	Channel channel[CHANNEL_SIZE]{}; // 개수 바뀔 수 있음.
};

class ClientLoginPacketTypeShowChannelRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeShowChannelRequest() : ClientLoginPacket(clientLoginPacketTypeShowChannelRequest) {}
	~ClientLoginPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class ClientLoginPacketTypeChannelInResponse : public ClientLoginPacket {
public:
	ClientLoginPacketTypeChannelInResponse() : ClientLoginPacket(clientLoginPacketTypeChannelInResponse) {}
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

class ClientLoginPacketTypeChannelInRequest : public ClientLoginPacket {
public:
	ClientLoginPacketTypeChannelInRequest() : ClientLoginPacket(clientLoginPacketTypeChannelInRequest) {}
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

/*

		  login <-> management

*/

class LoginManagementPacket : public BasePacket {
public:
	LoginManagementPacket(LoginPacketType _loginPacketType) : BasePacket(BasePacketType::basePacketTypeLogin), loginPacketType(_loginPacketType) {
		this->PacketTypeSerial(loginPacketType);
	}
	~LoginManagementPacket() {}
protected:
	LoginPacketType loginPacketType = loginPacketTypeNone;
};

class LoginManagementPacketTypeLoginResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeLoginResponse() : LoginManagementPacket(loginManagementPacketTypeLoginResponse) {}
	~LoginManagementPacketTypeLoginResponse() {}

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
	uint32 serialNumber = 0;
};

class LoginManagementPacketTypeLoginRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeLoginRequest() : LoginManagementPacket(loginManagementPacketTypeLoginRequest) {}
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

class LoginManagementPacketTypeLogoutRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeLogoutRequest() : LoginManagementPacket(loginManagementPacketTypeLogoutRequest) {}
	~LoginManagementPacketTypeLogoutRequest() {}

	virtual Buffer& Serialize() override {
		buffer << serialNumber;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> serialNumber;
	}
	uint32 serialNumber = 0;
};

class LoginManagementPacketTypeSignupResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeSignupResponse() : LoginManagementPacket(loginManagementPacketTypeSignupResponse) {}
	~LoginManagementPacketTypeSignupResponse() {}

	virtual Buffer& Serialize() override {
		buffer << flag;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		buffer >> flag;
	}

	bool flag = true;
};

class LoginManagementPacketTypeSignupRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeSignupRequest() : LoginManagementPacket(loginManagementPacketTypeSignupRequest) {}
	~LoginManagementPacketTypeSignupRequest() {}

	virtual Buffer& Serialize() override {
		buffer << userId;
		buffer << userPw;

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

class LoginManagementPacketTypeDeleteResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeDeleteResponse() : LoginManagementPacket(loginManagementPacketTypeDeleteResponse) {}
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

class LoginManagementPacketTypeDeleteRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeDeleteRequest() : LoginManagementPacket(loginManagementPacketTypeDeleteRequest) {}
	~LoginManagementPacketTypeDeleteRequest() {}

	virtual Buffer& Serialize() override {
		buffer << serialNumber;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> serialNumber;
	}

	uint32 serialNumber = 0;
};

class LoginManagementPacketTypeShowChannelResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeShowChannelResponse() : LoginManagementPacket(loginManagementPacketTypeShowChannelResponse) {}
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

class LoginManagementPacketTypeShowChannelRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeShowChannelRequest() : LoginManagementPacket(loginManagementPacketTypeShowChannelRequest) {}
	~LoginManagementPacketTypeShowChannelRequest() {}

	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};

class LoginManagementPacketTypeChannelInResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeChannelInResponse() : LoginManagementPacket(loginManagementPacketTypeChannelInResponse) {}
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

class LoginManagementPacketTypeChannelInRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeChannelInRequest() : LoginManagementPacket(loginManagementPacketTypeChannelInRequest) {}
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