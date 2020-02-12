#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
#define __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__

#include "BasePacket.h"

enum class PacketTypeSocialServer : char {
	none = 0,

	updatePlayerInfo,

	//Friend
	addFriendRequest,	
	addFriendResponse,	


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

	GPID m_gpid = 0;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;

		return;
	}
};

//Friend
struct SocialPacketServerAddFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerAddFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::addFriendRequest) {}

	GPID m_src;
	std::string m_destName;

	virtual Buffer& Serialize() {
		buffer << m_src;
		buffer << m_destName;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_src;
		buf >> m_destName;

		return;
	}
};
struct SocialPacketServerAddFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerAddFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::addFriendResponse) {}

	virtual Buffer& Serialize() {
	
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		
		return;
	}
};
#endif // !__SOCIALSERVER_BASE_SOCIAL_PACKET_H__
