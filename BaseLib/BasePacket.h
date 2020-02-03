#ifndef BASEPACKET_H
#define BASEPACKET_H

#include <memory>
#include <string>
#include <type_traits>

#include "ServerCommon.h"


typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

/*
	=== EXAMPLE ===
	대분류 -> BasePacketType basePacketType = (BasePacketType)TypeDeserial((char*&)buf);
	중분류 -> SocialPacketType socialPacketType = (SocialPacketType)TypeDeserial((char*&)buf);

	=== Deserialize ===
	ChatAllRequest chat{};
	chat.Deserialize(buf);

	=== Serialize ===
	char* buf = chat.Serialize();
*/

enum BasePacketType : char {
	basePacketTypeNone = 0,
	basePacketTypeLogin,
	basePacketTypeGame,
	basePacketTypeRoom,
	basePacketTypeShop,
	basePacketTypeRanking,
	basePacketTypeSocial,
	basePacketTypeSize,
};

class BasePacket {
public:
	BasePacket(BasePacketType _basePacketType) : basePacketType(_basePacketType) {
		this->PacketTypeSerial(basePacketType);
	}
	virtual ~BasePacket() {}

	virtual char* Serialize() = 0;
	virtual void Deserialize(Buffer& buf) = 0;

	BasePacketType GetBasePacketType() const {
		return basePacketType;
	}
	void SetBasePacketType(BasePacketType _type) {
		basePacketType = _type;
	}

protected:
#pragma region Serialize

	//Type -> Byte, (타입)
	inline void PacketTypeSerial(char _type) {
		buf << _type;
	}

	//String -> Byte, (데이터)
	inline void StringSerial(std::string _data) {
		buf << _data;
	}

	//Value -> Byte, (값)
	inline void TypeSerial(char _val) {
		buf << _val;
	}

	inline void TypeSerial(bool _val) {
		buf << _val;
	}

	inline void TypeSerial(int16 _val) {
		buf << _val;
	}

	inline void TypeSerial(int32 _val) {
		buf << _val;
	}

	inline void TypeSerial(int64 _val) {
		buf << _val;
	}
#pragma endregion
#pragma region Deserialize
	void DeserializeBuffer(Buffer& _buf) {
		buf = _buf;
	}
	//Byte -> String, (버퍼, 데이터)
	inline void StringDeserial(Buffer& _buf, std::string& _data) {
		_buf >> _data;
	}

	//Byte -> Value, (버퍼, 값)
	template<typename T_Arg>
	inline void TypeDeserial(Buffer& _buf, T_Arg& _val) {
		_buf >> _val;
	}
#pragma endregion
protected:
//	char buf[BUFFER_SIZE]{};
	Buffer buf;
private:
	BasePacketType basePacketType = basePacketTypeNone;
};

//Byte -> Type, 

/*
inline char PacketTypeDeserial(char* _buf) {
	if (nullptr == _buf) return NULL;
	if (1 < idx)
		idx = 0;
	char type = _buf[idx];
	++idx;
	return type;
}*/

inline char PacketTypeDeserial(Buffer& _buf) {
	char type;
	_buf >> type;
	return type;
}
#endif