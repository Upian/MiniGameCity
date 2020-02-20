#pragma once
#ifndef __GAMESERVER_SOCIAL_PACKET_H__
#define __GAMESERVER_SOCIAL_PACKET_H__
#include <list>
#include <tuple>
#include "BasePacket.h"
#include "ErrorType.h"
#include "Room.h"
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
	packetTypeSocialAddFriendRequest, //친추 요청
	packetTypeSocialAddFriendResponse,
	packetTypeSocialDeleteFriendRequest,
	packetTypeSocialDeleteFriendResponse,
	packetTypeSocialConfirmFriendRequest, //받은 친추 목록
	packetTypeSocialConfirmFriendResponse,
	packetTypeSocialAcceptFriendRequest, //친구 요청 수락
	packetTypeSocialAcceptFriendResponse,
	packetTypeSocialFriendListRequest, //친구 리스트
	packetTypeSocialFriendListResponse,
	packetTypeSocialInviteFriendRequest, //친구 게임 초대
	packetTypeSocialInviteFriendResponse, //가능 여부 판별하여 반환
	packetTypeSocialConfirmInviteFriendResponse, //Social -> Game -> client 참가 여부 묻기 위한 패킷


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
struct SocialGamePacketChatFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketChatFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialChatFriendRequest) {}

	std::string m_destName;
	std::string m_message;

	virtual Buffer& Serialize() {
		buffer << m_destName;
		buffer << m_message;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_destName;
		buf >> m_message;
		return;
	}
};
struct SocialGamePacketChatFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketChatFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialChatFriendResponse) {}
	
	std::string m_name;
	std::string m_message;
	bool m_isSender = false;

	virtual Buffer& Serialize() {
		buffer << m_name;
		buffer << m_message;
		buffer << m_isSender;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_name;
		buf >> m_message;
		buf >> m_isSender;

		return;
	}
};


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

struct SocialGamePacketDeleteFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketDeleteFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialDeleteFriendRequest) {}

	std::string m_name;

	virtual Buffer& Serialize() {
		buffer << m_name;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_name;
		return;
	}
};
struct SocialGamePacketDeleteFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketDeleteFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialDeleteFriendResponse) {}

	bool m_isSuccess = false;

	virtual Buffer& Serialize() {
		buffer << m_isSuccess;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buffer >> m_isSuccess;
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
struct SocialGamePacketAcceptFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketAcceptFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialAcceptFriendResponse) {}

	ErrorTypeAcceptFriend m_errorCode = ErrorTypeAcceptFriend::none;

	virtual Buffer& Serialize() {
		buffer << m_errorCode;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_errorCode;

		return;
	}
};

struct SocialGamePacketFriendListRequest : public BaseSocialGamePacket {
	SocialGamePacketFriendListRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialFriendListRequest) {}

	virtual Buffer& Serialize() {
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		return;
	}
};
struct SocialGamePacketFriendListResponse : public BaseSocialGamePacket {
	SocialGamePacketFriendListResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialFriendListResponse) {}

	__int16 m_size = 0;
	std::list<std::tuple<std::string, bool> > m_friends;

	virtual Buffer& Serialize() {
		m_size = m_friends.size();

		buffer << m_size;
		for (auto s : m_friends) {
			buffer << std::get<0>(s);
			buffer << std::get<1>(s);
		}
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		std::tuple<std::string, bool> temp;

		buf >> m_size;

		for (int i = 0; i < m_size; ++i) {
			buf >> std::get<0>(temp);
			buf >> std::get<1>(temp);
			m_friends.emplace_back(temp);
		}

		return;
	}
};

struct SocialGamePacketInviteFriendRequest : public BaseSocialGamePacket {
	SocialGamePacketInviteFriendRequest() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialInviteFriendRequest) {}

	std::string m_friendname;

	virtual Buffer& Serialize() override{
		buffer << m_friendname;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override{
		buf >> m_friendname;

		return;
	}
};
struct SocialGamePacketInviteFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketInviteFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialInviteFriendResponse) {}

	bool m_isSuccess = false;

	virtual Buffer& Serialize() override {
		buffer << m_isSuccess;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> m_isSuccess;

		return;
	}
};

struct SocialGamePacketConfirmInviteFriendResponse : public BaseSocialGamePacket {
	SocialGamePacketConfirmInviteFriendResponse() : BaseSocialGamePacket(PacketTypeSocialClient::packetTypeSocialConfirmInviteFriendResponse) {}
	
	int m_roomNumber;
	std::string m_name;
	std::string m_roomName;
	std::string m_ipAddress;
	int m_port = 0;
	RoomGameType m_gameMode = RoomGameType::GameTypeNone;

	virtual Buffer& Serialize() override {
		buffer << m_roomNumber;
		buffer << m_name;
		buffer << m_roomName;
		buffer << m_ipAddress;
		buffer << m_port;
		buffer << m_gameMode;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> m_roomNumber;
		buf >> m_name;
		buf >> m_roomName;
		buf >> m_ipAddress;
		buf >> m_port;
		buf >> m_gameMode;
		return;
	}
};
#endif // !__GAMESERVER_SOCIAL_PACKET_H__
