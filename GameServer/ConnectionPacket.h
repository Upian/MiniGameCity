#pragma once
#ifndef __GAMESERVER_PRE_LOAD_PACKET_H__
#define __GAMESERVER_PRE_LOAD_PACKET_H__

#include "BasePacket.h"

enum class PacketTypeConnection : char {
	packetTypeNone = 0,

	preLoadPlayerInfo,

	connectionRequest,
	connectionResponse,

	InviteTransferRequest,
	InviteTransferResponse,

	packetTypeCount,
};

struct BaseConnectionPacket : public BasePacket {
	BaseConnectionPacket(PacketTypeConnection type) : BasePacket(BasePacketType::basePacketTypeConnection), m_packetTypePreLoad(type) {
		this->PacketTypeSerial(m_packetTypePreLoad);
	}
	
protected:
	PacketTypeConnection m_packetTypePreLoad = PacketTypeConnection::packetTypeNone;
};

struct ConnectionPacketLoadPlayerInfo : public BaseConnectionPacket {
	ConnectionPacketLoadPlayerInfo() : BaseConnectionPacket(PacketTypeConnection::preLoadPlayerInfo) {}

	std::string m_playerName;

	virtual Buffer& Serialize() override {
		buffer << m_playerName;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_playerName;

	}
};
//After login connect game server
//client --> game server
struct ConnectionPacketConnectServerRequest : public BaseConnectionPacket {
	ConnectionPacketConnectServerRequest() : BaseConnectionPacket(PacketTypeConnection::connectionRequest) {}

	__int64 m_sessionId;

	virtual Buffer& Serialize() override {
		buffer << m_sessionId;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_sessionId;
	}
};


//Game server transfer when Invite friend
struct ConnectionPacketInviteTransferRequest : public BaseConnectionPacket {
	ConnectionPacketInviteTransferRequest() : BaseConnectionPacket(PacketTypeConnection::InviteTransferRequest) {}

	std::string m_ipAddress;
	int m_portNum;
	time_t m_sessionID = 0;

	virtual Buffer& Serialize() override {
		buffer << m_ipAddress;
		buffer << m_portNum;
		buffer << m_sessionID;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_ipAddress;
		_buf >> m_portNum;
		_buf >> m_sessionID;
	}
};
struct ConnectionPacketInviteTransferResponse : public BaseConnectionPacket {
	ConnectionPacketInviteTransferResponse() : BaseConnectionPacket(PacketTypeConnection::InviteTransferResponse) {}


	virtual Buffer& Serialize() override {

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {

	}
};






























#endif // !__GAMESERVER_PRE_LOAD_PACKET_H__
