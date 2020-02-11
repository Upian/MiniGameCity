#pragma once
#ifndef __GAMESERVER_SOCIAL_PACKET_H__
#define __GAMESERVER_SOCIAL_PACKET_H__

#include "BasePacket.h"

enum class PacketTypeSocialClient : char{
	packetTypeSocialNone = 0,
	//Chat
	packetTypeSocialChatNormalRequest, //Client -> Game server
	packetTypeSocialChatNormalResponse, //Game server -> All clients
	packetTypeSocialChatFriendRequest, //Client -> Game server
	packetTypeSocialChatFriendResponse, //Game server -> friend client
	packetTypeSocialChatGuildRequest, //Client -> Game server
	packetTypeSocialChatGuildResponse, //Game server -> All guild clients
	//Friend
	packetTypeSocialAddFriendRequest,
	packetTypeSocialAddFriendResponse,
	packetTypeSocialAcceptFriendRequest,
	packetTypeSocialAcceptFriendResponse,

	packetTypeSocialCount,
};

struct BaseSocialGamePacket : public BasePacket {
	BaseSocialGamePacket(PacketTypeSocialClient packetType) : BasePacket(BasePacketType::basePacketTypeSocial), m_packetTypeSocial(packetType) {
		this->PacketTypeSerial(m_packetTypeSocial);
	}
protected:
	PacketTypeSocialClient m_packetTypeSocial = PacketTypeSocialClient::packetTypeSocialNone;
};

//Normal chat
struct SocialGamePacketChatNormalRequest: public BaseSocialGamePacket{
	SocialGamePacketChatNormalRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialChatNormalRequest) {}
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
struct SocialGamePacketChatNormalResponse : public BaseSocialGamePacket {
	SocialGamePacketChatNormalResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialChatNormalResponse) {}

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

//Friend chat

//Guild chat

//Friend
struct SocialGamePacketAddFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketAddFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialAddFriendRequest) {}

	std::string m_name;

	virtual Buffer& Serialize() {

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {

		return;
	}
};
struct SocialGamePacketAddFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketAddFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialAddFriendResponse) {}

	virtual Buffer& Serialize() {

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {

		return;
	}

};
#endif // !__GAMESERVER_SOCIAL_PACKET_H__
