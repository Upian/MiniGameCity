#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__

#include <list>
#include "BasePacket.h"

enum ManagementPacketType : char {
	managementPacketTypeNone = 0,

	// login server <-> management sserver
	loginManagementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople), (unsigned int)GPID
	loginManagementPacketTypeShowChannelRequest, //nothing.
	loginManagementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	loginManagementPacketTypeChannelInRequest, //(string)channelName, (unsigned int)GPID

	// game server <-> management server
	gameManagementPacketTypeCurrentPeopleRequest, // (int32)currentPeople 
	gameManagementPacketTypeChannelInRequest, // (unsigned int)GPID
	registerServerInfo, //Game -> Management
	preLoadRequest, //Management -> Game
	updateServerInfo, //Game -> Management

	managementPacketTypeSize,
};

struct Channel {
	std::string _channelName;
	int32 _currentPeople = 0;
	int32 _maximumPeople = 0;

	Channel() {}
	Channel(std::string channelName, int32 currentPeople, int32 maximumPeople)
		: _channelName(channelName), _currentPeople(currentPeople), _maximumPeople(maximumPeople) {}
};

/*

		  login <-> management

*/

class LoginManagementPacket : public BasePacket {
public:
	LoginManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::loginPacketTypeManagement), managementPacketType(_managementPacketType) {
		this->PacketTypeSerial(managementPacketType);
	}
	~LoginManagementPacket() {}
protected:
	ManagementPacketType managementPacketType = managementPacketTypeNone;
};

class LoginManagementPacketTypeShowChannelResponse : public LoginManagementPacket {
public:
	LoginManagementPacketTypeShowChannelResponse() : LoginManagementPacket(loginManagementPacketTypeShowChannelResponse) {}
	~LoginManagementPacketTypeShowChannelResponse() {}

	virtual Buffer& Serialize() override {
		buffer << channelSize;
		for (auto s : channel) {
			buffer << s._channelName;
			buffer << s._currentPeople;
			buffer << s._maximumPeople;
		}
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelSize;
		Channel tmp;
		for (int i = 0; i < channel.size(); ++i) {
			_buf >> tmp._channelName;
			_buf >> tmp._currentPeople;
			_buf >> tmp._maximumPeople;
			channel.emplace_back(tmp);
		}
		_buf >> gpid;
	}

	int32 channelSize;
	std::list<Channel> channel;
	uint32 gpid = 0;
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
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> flag;
		_buf >> ip;
		_buf >> port;
		_buf >> gpid;
	}

	bool flag = true;
	std::string ip;
	int16 port = 0;
	uint32 gpid = 0;
};

class LoginManagementPacketTypeChannelInRequest : public LoginManagementPacket {
public:
	LoginManagementPacketTypeChannelInRequest() : LoginManagementPacket(loginManagementPacketTypeChannelInRequest) {}
	~LoginManagementPacketTypeChannelInRequest() {}

	virtual Buffer& Serialize() override {
		buffer << channelName;
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> channelName;
		_buf >> gpid;
	}

	std::string channelName;
	uint32 gpid;
};

/////////////////////////////////////////
struct GameToManagementPacket : public BasePacket {
	GameToManagementPacket(ManagementPacketType type) : BasePacket(BasePacketType::basePacketTypeGameToManagementServer), m_packetType(type) {
		this->PacketTypeSerial(m_packetType);
	}
protected:
	ManagementPacketType m_packetType = ManagementPacketType::managementPacketTypeNone;
};

struct GameToManagementResistarServerInfo : public GameToManagementPacket {
	GameToManagementResistarServerInfo() : GameToManagementPacket(ManagementPacketType::registerServerInfo) {}

	int m_maxPlayer = 0;
	
	virtual Buffer& Serialize() override {
		buffer << m_maxPlayer;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_maxPlayer;
	}
};

struct GameToManagementPreLoadRequest : public GameToManagementPacket {
	GameToManagementPreLoadRequest() : GameToManagementPacket(ManagementPacketType::preLoadRequest) {}

	unsigned __int32 m_gpid;

	virtual Buffer& Serialize() override {
		buffer << m_gpid;
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_gpid;
	}
};

/*

		  game server <-> management server

*/
class GameManagementPacket : public BasePacket {
public:
	GameManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::basePacketTypeGameToManagementServer), managementPacketType(_managementPacketType) {
		this->PacketTypeSerial(managementPacketType);
	}
	~GameManagementPacket() {}
protected:
	ManagementPacketType managementPacketType = managementPacketTypeNone;
};

class GameManagementPacketTypeCurrentPeopleRequest : public GameManagementPacket {
public:
	GameManagementPacketTypeCurrentPeopleRequest() : GameManagementPacket(gameManagementPacketTypeCurrentPeopleRequest) {}
	~GameManagementPacketTypeCurrentPeopleRequest() {}

	virtual Buffer& Serialize() override {
		buffer << currentPeople;
		
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> currentPeople;
	}

	int32 currentPeople = 0;
};

class GameManagementPacketTypeChannelInRequest : public GameManagementPacket {
public:
	GameManagementPacketTypeChannelInRequest() : GameManagementPacket(gameManagementPacketTypeChannelInRequest) {}
	~GameManagementPacketTypeChannelInRequest() {}

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