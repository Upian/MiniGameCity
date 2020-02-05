#pragma once
#ifndef __GAMESERVER_ROOM_PACKET_H__
#define __GAMESERVER_ROOM_PACKET_H__

#include <string>
#include <list>
#include "BasePacket.h"
#include "Player.h"

//1byte
enum class PacketTypeRoom : char {
	packetTypeRoomNone = 0,

	packetTypeRoomMakeRoomRequest, //Client -> GameServer
	packetTypeRoomMakeRoomResponse, //GameServer -> Client

	packetTypeRoomRoomListRequest, //Client -> GameServer
	packetTypeRoomRoomListResponse, //GameServer -> Client

	packetTypeRoomEnterRoomRequest, //Client -> GameServer
	packetTypeRoomEnterRoomResponse, //GameServer -> Client

	packetTypeRoomRoomInfo, //GameServer -> Client Broadcast (also used EnterRoomResponse)

	packetTypeRoomCount,
};


struct BaseRoomPacket : public BasePacket{
	BaseRoomPacket(PacketTypeRoom packetType) : BasePacket(BasePacketType::basePacketTypeRoom), m_packetTypeRoom(packetType){
		this->PacketTypeSerial(m_packetTypeRoom);
	}
protected:
	PacketTypeRoom m_packetTypeRoom = PacketTypeRoom::packetTypeRoomNone;
};

// Make Room
struct RoomPacketMakeRoomRequest : public BaseRoomPacket {
	RoomPacketMakeRoomRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomMakeRoomRequest) {}

	std::string		m_roomName;
	int16			m_maxPlayer = 0;
	int16			m_password = 0;

	virtual Buffer& Serialize() override {

		buffer << m_roomName;
		buffer << m_maxPlayer;
		buffer << m_password;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
//		DeserializeBuffer(_buf);
		_buf >> m_roomName;
		_buf >> m_maxPlayer;
		_buf >> m_password;
	}
};
struct RoomPacketMakeRoomResponse : public BaseRoomPacket {
	RoomPacketMakeRoomResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomMakeRoomResponse) {}

	bool		m_success = false;
	int32		m_roomNumber = 0;

	virtual Buffer& Serialize() override {
		buffer << m_success;
		buffer << m_roomNumber;
		
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_success;
		_buf >> m_roomNumber;
	}
};

// Room list
struct RoomPacketRoomListRequest : public BaseRoomPacket {
	RoomPacketRoomListRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomListRequest) {}

	__int16 m_page = 0;
	virtual Buffer& Serialize() override {
		buffer << m_page;
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_page;
	}
};
struct RoomPacketRoomListResponse : public BaseRoomPacket {
	RoomPacketRoomListResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomListResponse) {}
private:
	struct RoomInfo {
		RoomInfo(int num, int plycnt, int maxcount, bool pass, std::string& str) :
		number(num),
		playerCount(plycnt),
		maxPlayerCount(maxcount),
		password(pass),
		roomName(str)
		{}
		__int16			number = 0;
		__int16			playerCount = 0;
		__int16			maxPlayerCount = 0;
		bool			password = false;
		std::string		roomName;
	};

public:
	std::list<RoomInfo> m_roomList;
	__int16				m_listCount = 0;
	virtual Buffer& Serialize() override {
		m_listCount = static_cast<__int16>(m_roomList.size());

		buffer << m_listCount;

		for (RoomInfo info : m_roomList) {
			buffer << info.number;
			buffer << info.playerCount;
			buffer << info.maxPlayerCount;
			buffer << info.password;
			buffer << info.roomName;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		__int16			tempNumber = 0;
		__int16			tempPlayerCount = 0;
		__int16			tempMaxPlayerCount = 0;
		bool			tempPassword = false;
		std::string		tempStr;

		_buf >> m_listCount;

		for(int i = 0; i < m_listCount; ++i) {  
			_buf >> tempNumber;
			_buf >> tempPlayerCount;
			_buf >> tempMaxPlayerCount;
			_buf >> tempPassword;
			_buf >> tempStr;
			m_roomList.emplace_back(
				tempNumber,
				tempPlayerCount,
				tempMaxPlayerCount,
				tempPassword,
				tempStr
			);
		}
	}
};

//Enter room
enum class ErrorTypeEnterRoom : char {
	errorTypeNone = 0,
	
	errorTypeMaxPlayer,
	errorTypeGameStart,
	errorTypeNotExistRoom,
	errorTypeWrongPassword,

	errorTypeCount,
};
struct RoomPacketEnterRoomRequest : public BaseRoomPacket {
	RoomPacketEnterRoomRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomEnterRoomRequest) {}

	__int32 m_roomNumber = 0;
	__int16 m_password = 0;
	virtual Buffer& Serialize() override{
		buffer << m_roomNumber;
		buffer << m_password;
		
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_roomNumber;
		_buf >> m_password;
	}

};
struct RoomPacketEnterRoomResponse : public BaseRoomPacket {
	RoomPacketEnterRoomResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomEnterRoomResponse) {}

	bool m_isSuccess = false;
	ErrorTypeEnterRoom m_errorType = ErrorTypeEnterRoom::errorTypeNone;

	virtual Buffer& Serialize() override {
		buffer << m_isSuccess;
		buffer << m_errorType;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_isSuccess;
		_buf >> m_errorType;
	}
};

#endif // !__GAMESERVER_ROOM_PACKET_H__
