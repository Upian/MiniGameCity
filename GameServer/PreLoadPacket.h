#pragma once
#ifndef __GAMESERVER_PRE_LOAD_PACKET_H__
#define __GAMESERVER_PRE_LOAD_PACKET_H__

#include "BasePacket.h"

enum class PacketTypePreLoad : char {
	packetTypeNone = 0,

	preLoadPlayerInfo,

	packetTypeCount,
};

struct BasePreLoadPacket : public BasePacket {
	BasePreLoadPacket(PacketTypePreLoad type) : BasePacket(BasePacketType::basePacketTypePreLoad), m_packetTypePreLoad(type) {
		this->PacketTypeSerial(m_packetTypePreLoad);
	}
	
protected:
	PacketTypePreLoad m_packetTypePreLoad = PacketTypePreLoad::packetTypeNone;
};

struct PreLoadPacketLoadPlayerInfo : public BasePreLoadPacket {
	PreLoadPacketLoadPlayerInfo() : BasePreLoadPacket(PacketTypePreLoad::preLoadPlayerInfo) {}

	std::string m_playerName;

	virtual Buffer& Serialize() override {
		buffer << m_playerName;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_playerName;

	}
};
































#endif // !__GAMESERVER_PRE_LOAD_PACKET_H__
