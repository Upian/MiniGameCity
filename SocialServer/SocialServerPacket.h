#pragma once
#ifndef __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__
#define __SOCIALSERVER_SOCIAL_SERVER_PACKET_H__

#include "BasePacket.h"
using GPID = unsigned __int32;
enum class PacketTypeSocialServer : char {
	none = 0,

	updatePlayerLogin,
	updatePlayerLogout,
	//Friend
	addFriendRequest,			//src player to server (Client -> Server)
	addFriendResponse,			
	confirmAddFriendRequest,	//dest player to server (client -> server)
	confirmAddFriendResponse,	


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

struct SocialPacketServerConfirmAddFriendRequest : public BaseSocialServerPacket {
	SocialPacketServerConfirmAddFriendRequest() : BaseSocialServerPacket(PacketTypeSocialServer::confirmAddFriendRequest) {}

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
struct SocialPacketServerConfirmAddFriendResponse : public BaseSocialServerPacket {
	SocialPacketServerConfirmAddFriendResponse() : BaseSocialServerPacket(PacketTypeSocialServer::confirmAddFriendResponse) {}
	
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
		
		return;
	}
};
#endif // !__SOCIALSERVER_BASE_SOCIAL_PACKET_H__
