#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
#define __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__

#include <list>
#include <tuple>
#include "BasePacket.h"
#include "ErrorType.h"
using GPID = unsigned __int32;
enum class PacketTypeSocialServer : char {
	none = 0,

	//Update server info
	registerServerInfo, //send game servers ip address and port number to social server 

	updatePlayerLogin,
	updatePlayerLogout,
	//Friend
	addFriendRequest,			//src player to server (Client -> Server)
	addFriendResponse,			
	deleteFriendRequest,		//src
	deleteFriendResponse,
	confirmFriendRequest,	//dest player to server (client -> server)
	confirmFriendResponse,	
	acceptFriendRequest, //accept friend request
	acceptFriendResponse, //accept friend request
	friendListRequest, //Get players friends list
	friendListResponse,
	chatFriendRequest,
	chatFriendResponse,
	InviteRequestFriendRequest, // src client -> game server -> social server
	InviteResponseFriendResponse, // src client <- game server <- social server
	InviteConfirmFriendRequest, //social server -> game server -> dest client
	InviteConfirmFriendResponse,

	//Guild


	count,
};

struct BaseSocialServerPacket : public BasePacket {
	BaseSocialServerPacket(PacketTypeSocialServer type) : BasePacket(BasePacketType::basePacketTypeGameToSocialServer), m_packetType(type) {
		this->PacketTypeSerial(m_packetType);
	}

protected:
	PacketTypeSocialServer m_packetType = PacketTypeSocialServer::none;
};

//Update server info
struct SocialPacketServerRegisterServerInfo : public BaseSocialServerPacket {
	SocialPacketServerRegisterServerInfo() : BaseSocialServerPacket(PacketTypeSocialServer::registerServerInfo) {}

	std::string m_ipAddress;
	__int32 m_portNum;

	virtual Buffer& Serialize() override {
		buffer << m_ipAddress;
		buffer << m_portNum;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		buf >> m_ipAddress;
		buf >> m_portNum;
	}
};


//Update player info
struct SocialPacketServerUpdatePlayerLogin : public BaseSocialServerPacket {
	SocialPacketServerUpdatePlayerLogin() : BaseSocialServerPacket(PacketTypeSocialServer::updatePlayerLogin) {}

	GPID m_gpid = 0;
	std::string m_name; //#Test

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_name;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_name;
		return;
	}
};
struct SocialPacketServerUpdatePlayerLogout : public BaseSocialServerPacket {
	SocialPacketServerUpdatePlayerLogout() : BaseSocialServerPacket(PacketTypeSocialServer::updatePlayerLogout) {}

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

	GPID m_srcGpid;
	std::string m_destName;

	virtual Buffer& Serialize() {
		buffer << m_srcGpid;
		buffer << m_destName;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_srcGpid;
		buf >> m_destName;

		return;
	}
};
struct SocialPacketServerAddFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerAddFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::addFriendResponse) {}

	GPID m_gpid = 0;
	bool m_success = false;
	ErrorTypeAddFriend m_errorType = ErrorTypeAddFriend::none;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_success;
		buffer << m_errorType;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_success;
		buf >> m_errorType;

		return;
	}
};

struct SocialPacketServerDeleteFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerDeleteFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::deleteFriendRequest) {}

	GPID m_gpid = 0;
	std::string m_name;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_name;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_name;
		return;
	}
};

struct SocialPacketServerDeleteFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerDeleteFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::deleteFriendResponse) {}

	bool m_isSuccess = false;
	GPID m_gpid = 0;

	virtual Buffer& Serialize() {
		buffer << m_isSuccess;
		buffer << m_gpid;
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_isSuccess;
		buf >> m_gpid;
		return;
	}
};

struct SocialPacketServerConfirmFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerConfirmFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::confirmFriendRequest) {}

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
struct SocialPacketServerConfirmFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerConfirmFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::confirmFriendResponse) {}
	
	GPID m_gpid = 0;
	__int16 m_size = 0;
	std::list<std::string> m_names;

	virtual Buffer& Serialize() {
		m_size = static_cast<__int16>(m_names.size());

		buffer << m_gpid;
		buffer << m_size;
		for (std::string name : m_names) {
			buffer << name;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		std::string tempName;

		buf >> m_gpid;
		buf >> m_size;
		for (int i = 0; i < m_size; ++i) {
			buf >> tempName;
			m_names.emplace_back(tempName);
		}
		return;
	}
};

struct SocialPacketServerAcceptFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerAcceptFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::acceptFriendRequest) {}

	GPID m_gpid;
	bool m_isAccept = false;
	std::string m_name;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_isAccept;
		buffer << m_name;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_isAccept;
		buf >> m_name;

		return;
	}
};
struct SocialPacketServerAcceptFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerAcceptFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::acceptFriendResponse) {}

	GPID m_gpid = 0;
	bool m_isSuccess = false;
	ErrorTypeAcceptFriend m_errorCode = ErrorTypeAcceptFriend::none;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_isSuccess;
		buffer << m_errorCode;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_isSuccess;
		buf >> m_errorCode;

		return;
	}
};

struct SocialPacketServerFriendListRequest : public BaseSocialServerPacket {
	SocialPacketServerFriendListRequest() : BaseSocialServerPacket(PacketTypeSocialServer::friendListRequest) {}
	
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
struct SocialPacketServerFriendListResponse : public BaseSocialServerPacket {
	SocialPacketServerFriendListResponse() : BaseSocialServerPacket(PacketTypeSocialServer::friendListResponse) {}

	GPID m_gpid = 0;
	__int16 m_size = 0;
	std::list<std::tuple<std::string, bool> > m_friends;

	virtual Buffer& Serialize() {
		m_size = m_friends.size();

		buffer << m_gpid;
		buffer << m_size;
		for (auto s : m_friends) {
			buffer << std::get<0>(s);
			buffer << std::get<1>(s);
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_size;

		std::tuple<std::string, bool> tempInfo;
		for (int i = 0; i < m_size; ++i) {
			buf >> std::get<0>(tempInfo);
			buf >> std::get<1>(tempInfo);
			m_friends.emplace_back(tempInfo);
		}
		return;
	}
};

struct SocialPacketServerChatFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerChatFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::chatFriendRequest) {}

	GPID m_srcGpid;
	std::string m_destName;
	std::string m_message;

	virtual Buffer& Serialize() {
		buffer << m_srcGpid;
		buffer << m_destName;
		buffer << m_message;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_srcGpid;
		buf >> m_destName;
		buf >> m_message;
		return;
	}
};
struct SocialPacketServerChatFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerChatFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::chatFriendResponse) {}

	GPID m_gpid = 0;
	std::string m_name;
	std::string m_message;
	bool m_isSender = false;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_name;
		buffer << m_message;
		buffer << m_isSender;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_name;
		buf >> m_message;
		buf >> m_isSender;
		return;
	}
};

struct SocialPacketServerInviteFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerInviteFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::InviteRequestFriendRequest) {}

	GPID m_gpid = 0;
	std::string m_roomName;
	std::string m_friendName;

	virtual Buffer& Serialize() {
		buffer << m_gpid;
		buffer << m_roomName;
		buffer << m_friendName;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_roomName;
		buf >> m_friendName;

	}
};

#endif // !__SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
