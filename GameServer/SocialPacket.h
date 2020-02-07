#pragma once
#ifndef __GAMESERVER_SOCIAL_PACKET_H__
#define __GAMESERVER_SOCIAL_PACKET_H__

#include "BasePacket.h"

enum class  PacketTypeSocial : char{
	packetTypeSocialNone,

	packetTypeSocialChatNormal,
	packetTypeSocialChatFriend,
	packetTypeSocialChatGuild,

	packetTypeSocialCount,
};

struct BaseSocialPacket : public BasePacket {
	BaseSocialPacket(PacketTypeSocial packetType) : BasePacket(BasePacketType::basePacketTypeSocial), m_packetTypeSocial(packetType) {
		this->PacketTypeSerial(m_packetTypeSocial);
	}
protected:
	PacketTypeSocial m_packetTypeSocial = PacketTypeSocial::packetTypeSocialNone;
};

//Normal chat
struct SocialPacketChatNormalRequest: public BaseSocialPacket{
	std::string m_message;

	virtual Buffer& Serialize() {
		buffer << m_message;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_message;
		
		return;
	}
};


#endif // !__GAMESERVER_SOCIAL_PACKET_H__
