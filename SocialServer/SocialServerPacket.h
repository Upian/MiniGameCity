#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
#define __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__

#include "BasePacket.h"

enum class PacketTypeSocialServer {
	none = 0,

	updatePlayerInfo,

	//Friend

	//Guild


	count,
};

struct BaseSocialServerPacket : public BasePacket {
	BaseSocialServerPacket(PacketTypeSocialServer type) : BasePacket(BasePacketType::basePacketTypeSocialServer), m_packetType(type) {
		this->PacketTypeSerial(m_packetType);
	}

protected:
	PacketTypeSocialServer m_packetType = PacketTypeSocialServer::none;
};

//Update player info
struct SocialPacketServerUpdatePlayerInfo : public BaseSocialServerPacket {
	SocialPacketServerUpdatePlayerInfo() : BaseSocialServerPacket(PacketTypeSocialServer::updatePlayerInfo) {}

	__int32 m_test;

	virtual Buffer& Serialize() {
		buffer << m_test;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_test;

		return;
	}
};


#endif // !__SOCIALSERVER_BASE_SOCIAL_PACKET_H__
