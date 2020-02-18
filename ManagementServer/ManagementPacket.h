#ifndef __MANAGEMENT_PACKET_H__
#define __MANAGEMENT_PACKET_H__
#include "LoginPacket.h"
//#include "..\DBCache\DBCachePacket.h"

// must go to ini file 
#define CHANNEL_SIZE 4 // test

enum ManagementPacketType : char {
	managementPacketTypeNone = 0,

	// login server <-> management sserver
	loginManagementPacketTypeShowChannelResponse, //(channel(string, int, int)channelName, numberOfPeople, limitOfPeople), (unsigned int)GPID
	loginManagementPacketTypeShowChannelRequest, //nothing.
	loginManagementPacketTypeChannelInResponse, //(bool)flag, (string)ip, (int)port
	loginManagementPacketTypeChannelInRequest, //(string)channelName, (unsigned int)GPID

	// game server <-> management server
	gameManagementPacketTypeCurrentPeopleRequest, // (int32)currentPeople 

	managementPacketTypeSize,
};

//struct Channel {
//	std::string channelName;
//	int32 numberOfPeople = 0;
//	int32 limitOfPeople = 0;
//};

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
		buffer << gpid;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		for (int i = 0; i < CHANNEL_SIZE; ++i) {
			_buf >> channel[i].channelName;
			_buf >> channel[i].numberOfPeople;
			_buf >> channel[i].limitOfPeople;
		}
		_buf >> gpid;
	}

	Channel channel[CHANNEL_SIZE]{}; // 개수 바뀔 수 있음.
	uint32 gpid = 0;
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

/*

		  game server <-> management server

*/
class GameManagementPacket : public BasePacket {
public:
	GameManagementPacket(ManagementPacketType _managementPacketType) : BasePacket(BasePacketType::gamePacketTypeManagement), managementPacketType(_managementPacketType) {
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

#endif