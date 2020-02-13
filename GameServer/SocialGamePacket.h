#pragma once
#ifndef __GAMESERVER_SOCIAL_PACKET_H__
#define __GAMESERVER_SOCIAL_PACKET_H__
#include <list>

#include "BasePacket.h"
#include "ErrorType.h"
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
	packetTypeSocialAddFriendRequest, //模眠 夸没
	packetTypeSocialAddFriendResponse,
	packetTypeSocialConfirmFriendRequest, //罐篮 模眠 格废
	packetTypeSocialConfirmFriendResponse,
	packetTypeSocialAcceptFriendRequest, //模备 夸没 荐遏
	packetTypeSocialAcceptFriendResponse,
	packetTypeSocialFriendListRequest, //模备 府胶飘
	packetTypeSocialFriendListResponse,

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

	std::string m_destName;

	virtual Buffer& Serialize() {
		buffer << m_destName;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_destName;

		return;
	}
};
struct SocialGamePacketAddFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketAddFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialAddFriendResponse) {}

	bool m_success = false;
	ErrorTypeAddFriend m_errorCode = ErrorTypeAddFriend::none;
	virtual Buffer& Serialize() {
		buffer << m_success;
		buffer << m_errorCode;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_success;
		buf >> m_errorCode;

		return;
	}

};

struct SocialGamePacketConfirmFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketConfirmFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialConfirmFriendRequest) {}

	virtual Buffer& Serialize() {
	

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
	

		return;
	}
};
struct SocialGamePacketConfirmFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketConfirmFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialConfirmFriendResponse) {}

	__int16 m_size = 0;
	std::list<std::string> m_names;
	
	virtual Buffer& Serialize() {
		m_size = static_cast<__int16>(m_names.size());

		buffer << m_size;
		for (std::string name : m_names) {
			buffer << name;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		std::string tempName;

		buf >> m_size;
		for (int i = 0; i < m_size; ++i) {
			buf >> tempName;
			m_names.emplace_back(tempName);
		}
		return;
	}
};

struct SocialGamePacketAcceptFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketAcceptFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialAcceptFriendRequest) {}

	bool m_isAccept = false;
	std::string m_name;

	virtual Buffer& Serialize() {
		buffer << m_isAccept;
		buffer << m_name;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_isAccept;
		buf >> m_name;
	}
};
#endif // !__GAMESERVER_SOCIAL_PACKET_H__
