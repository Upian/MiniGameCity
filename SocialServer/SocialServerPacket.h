#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
#define __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__

#include <list>

#include "BasePacket.h"
#include "ErrorType.h"
using GPID = unsigned __int32;
enum class PacketTypeSocialServer : char {
	none = 0,

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
	std::list<std::string> m_names;

	virtual Buffer& Serialize() {
		m_size = m_names.size();

		buffer << m_gpid;
		buffer << m_size;
		for (auto s : m_names) {
			buffer << s;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_gpid;
		buf >> m_size;

		std::string tempName;
		for (int i = 0; i < m_size; ++i) {
			buf >> tempName;
			m_names.emplace_back(tempName);
		}
		return;
	}
};
#endif // !__SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
