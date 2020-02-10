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

	packetTypeRoomLeaveRoomRequest, //Client -> GameServer
	packetTypeRoomLeaveRoomResponse, //GameServer -> Client

	packetTypeRoomRoomInfoRequest,
	packetTypeRoomRoomInfoResponse, //GameServer -> Client Broadcast (It will be send when new player enter the room)

	packetTypeRoomToggleReadyRequest, //Client -> GameServer
	packetTypeRoomToggleReadyResponse, //GameServer -> Client (To tell)

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
	int16			m_maximumPlayer = 0;
	int16			m_password = 0;

	virtual Buffer& Serialize() override {

		buffer << m_roomName;
		buffer << m_maximumPlayer;
		buffer << m_password;

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {

		buf >> m_roomName;
		buf >> m_maximumPlayer;
		buf >> m_password;
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
	virtual void Deserialize(Buffer& buf) override {
		buf >> m_success;
		buf >> m_roomNumber;
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
		RoomInfo(int num, __int64 time , int plycnt, int maxcount, bool gameStart, bool pass, std::string& str) :
		number(num),
		createdTime(time),
		playerCount(plycnt),
		maxPlayerCount(maxcount),
		isGameStart(gameStart),
		password(pass),
		roomName(str)
		{}

		__int16			number = 0;
		__int64			createdTime = 0;
		__int16			playerCount = 0;
		__int16			maxPlayerCount = 0;
		bool			isGameStart = false;
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
			buffer << info.createdTime;
			buffer << info.playerCount;
			buffer << info.maxPlayerCount;
			buffer << info.isGameStart;
			buffer << info.password;
			buffer << info.roomName;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		__int16			tempNumber = 0;
		__int64			tempTime = 0;
		__int16			tempPlayerCount = 0;
		__int16			tempMaxPlayerCount = 0;
		bool			tempIsGameStart = false;
		bool			tempPassword = false;
		std::string		tempStr;

		buf >> m_listCount;

		for(int i = 0; i < m_listCount; ++i) {  
			buf >> tempNumber;
			buf >> tempTime;
			buf >> tempPlayerCount;
			buf >> tempMaxPlayerCount;
			buf >> tempIsGameStart;
			buf >> tempPassword;
			buf >> tempStr;
			m_roomList.emplace_back(
				tempNumber,
				tempTime,
				tempPlayerCount,
				tempMaxPlayerCount,
				tempIsGameStart,
				tempPassword,
				tempStr
			);
		}
	}
};

//Enter room
enum class ErrorTypeEnterRoom : char {
	errorTypeNone = 0,

	errorTypeNotExistRoom,
	errorTypeWrongPassword,
	errorTypeAlreadyIncluded,
	errorTypeGameStart,
	errorTypeMaxPlayer,
	errorTypeCanNotEnterRoom,
	errorTypePlayerLogout,

	errorTypeCount,
};
struct RoomPacketEnterRoomRequest : public BaseRoomPacket {
	RoomPacketEnterRoomRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomEnterRoomRequest) {}

	__int32 m_roomNumber = 0;
	__int64 m_roomTime = 0;
	__int16 m_password = 0;
	virtual Buffer& Serialize() override{
		buffer << m_roomNumber;
		buffer << m_roomTime;
		buffer << m_password;
		
		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_roomNumber;
		_buf >> m_roomTime;
		_buf >> m_password;
	}

};
struct RoomPacketEnterRoomResponse : public BaseRoomPacket {
	RoomPacketEnterRoomResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomEnterRoomResponse) {}

	bool m_isSuccess = true;
	__int16 m_positionIndex = 0;
	ErrorTypeEnterRoom m_errorType = ErrorTypeEnterRoom::errorTypeNone;

	virtual Buffer& Serialize() override {
		buffer << m_isSuccess;
		buffer << m_positionIndex;
		buffer << m_errorType;

		return buffer;
	}
	virtual void Deserialize(Buffer& _buf) override {
		_buf >> m_isSuccess;
		_buf >> m_positionIndex;
		_buf >> m_errorType;
	}
};

//Leave room
struct RoomPacketLeaveRoomRequest : public BaseRoomPacket {
	RoomPacketLeaveRoomRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomLeaveRoomRequest) {}

	virtual Buffer& Serialize() override { return buffer; }
	virtual void Deserialize(Buffer& buf) override {}
};
struct RoomPacketLeaveRoomResponse : public BaseRoomPacket {
	RoomPacketLeaveRoomResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomLeaveRoomResponse) {}

	bool m_isSuccess = true;

	virtual Buffer& Serialize() override {
		buffer << m_isSuccess;
		
		return buffer;
	}
	virtual void Deserialize(Buffer& buf) {
		buf >> m_isSuccess;
	}
};

//packetTypeRoomRoomInfo
struct RoomPacketRoomInfoRequest : public BaseRoomPacket {
	RoomPacketRoomInfoRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomInfoRequest) {}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void  Deserialize(Buffer& buf) override {

	}
};
struct RoomPacketRoomInfoResponse : public BaseRoomPacket {
	RoomPacketRoomInfoResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomRoomInfoResponse) {}
private:
	struct PlayerInfo {
		explicit PlayerInfo(__int16 position, bool master, std::string name, __int32 index, bool ready) :
		positiontIndex(position),
		isRoomMaster(master),
		nickName(name),
		imageIndex(index),
		isPlayerReady(ready)
		{}

		__int16			positiontIndex;
		bool			isRoomMaster;
		std::string		nickName;
		__int32			imageIndex;
		bool			isPlayerReady;
	};

public:
	std::list<PlayerInfo> m_players;
	__int16 m_listCount = 0;

	virtual Buffer& Serialize() override {
		m_listCount = static_cast<__int16>(m_players.size());

		buffer << m_listCount;
		for (PlayerInfo info : m_players) {
			buffer << info.positiontIndex;
			buffer << info.isRoomMaster;
			buffer << info.nickName;
			buffer << info.imageIndex;
			buffer << info.isPlayerReady;
		}

		return buffer;
	}
	virtual void Deserialize(Buffer& buf) override {
		__int16 tempPositionIndex;
		bool tempIsRoomMaster;
		std::string tempNickName;
		__int32 tempImageIndex;
		bool tempIsPlayerReady;

		buf >> m_listCount;
		for (int i = 0; i < m_listCount; ++i) {
			buf >> tempPositionIndex;
			buf >> tempIsRoomMaster;
			buf >> tempNickName;
			buf >> tempImageIndex;
			buf >> tempIsPlayerReady;
			m_players.emplace_back(
				tempPositionIndex,
				tempIsRoomMaster,
				tempNickName,
				tempImageIndex,
				tempIsPlayerReady
			);
		}
	}
};

//ready
struct RoomPacketToggleReadyRequest : public BaseRoomPacket {
	RoomPacketToggleReadyRequest() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomToggleReadyRequest) {}

	virtual Buffer& Serialize() override {
		return buffer;
	}
	virtual void  Deserialize(Buffer& buf) override {

	}
};
struct RoomPacketToggleReadyResponse : public BaseRoomPacket {
	RoomPacketToggleReadyResponse() : BaseRoomPacket(PacketTypeRoom::packetTypeRoomToggleReadyResponse) {}

	bool m_isReady = false;

	virtual Buffer& Serialize() override {
		buffer << m_isReady;

		return buffer;
	}
	virtual void  Deserialize(Buffer& buf) override {
		buf >> m_isReady;
	}
};
//
#endif // !__GAMESERVER_ROOM_PACKET_H__
