#pragma once
#ifndef __GAMESERVER_SOCIAL_PACKET_H__
#define __GAMESERVER_SOCIAL_PACKET_H__

#include "BasePacket.h"
/**/
enum class  PacketTypeSocial : char{
	packetTypeSocialNone,

	packetTypeSocialChatNormalRequest, //Client -> Game server
	packetTypeSocialChatNormalResponse, //Game server -> All clients

	packetTypeSocialChatFriendRequest, //Client -> Game server
	packetTypeSocialChatFriendResponse, //Game server -> friend client

	packetTypeSocialChatGuildRequest, //Client -> Game server
	packetTypeSocialChatGuildResponse, //Game server -> All guild clients

	packetTypeSocialCount,
};

struct BaseSocialPacket : public BasePacket {
	BaseSocialPacket(PacketTypeSocial packetType) : BasePacket(BasePacketType::clientBasePacketTypeSocial), m_packetTypeSocial(packetType) {
		this->PacketTypeSerial(m_packetTypeSocial);
	}
protected:
	PacketTypeSocial m_packetTypeSocial = PacketTypeSocial::packetTypeSocialNone;
};

//Normal chat
struct SocialPacketChatNormalRequest: public BaseSocialPacket{
	SocialPacketChatNormalRequest() : BaseSocialPacket(PacketTypeSocial::packetTypeSocialChatNormalRequest) {}
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
struct SocialPacketChatNormalResponse : public BaseSocialPacket {
	SocialPacketChatNormalResponse() : BaseSocialPacket(PacketTypeSocial::packetTypeSocialChatNormalResponse) {}

	std::string m_name;
	std::string m_message;

	virtual Buffer& Serialize() {
		buffer << m_name;
		buffer << m_message;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_name;
		buf >> m_message;

		return;
	}
};

#endif // !__GAMESERVER_SOCIAL_PACKET_H__