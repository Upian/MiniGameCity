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
	��з� -> BasePacketType basePacketType = (BasePacketType)TypeDeserial((char*&)buffer);
	�ߺз� -> SocialPacketType socialPacketType = (SocialPacketType)TypeDeserial((char*&)buffer);

	=== Deserialize ===
	ChatAllRequest chat{};
	chat.Deserialize(buffer);

	=== Serialize ===
	char* buffer = chat.Serialize();
*/



enum class BasePacketType : char {
	basePacketTypeNone = 0,
	// client to server packet
	basePacketTypeLogin,
	basePacketTypeRoom,
	basePacketTypeGame,
	basePacketTypeMarket,
	basePacketTypeRanking,
	basePacketTypeSocial,

	basePacketTypeCount
};

class BasePacket {
public:
	BasePacket(BasePacketType _basePacketType) : basePacketType(_basePacketType) {
		this->PacketTypeSerial(basePacketType);
	}
	virtual ~BasePacket() {}

	virtual Buffer& Serialize() = 0;
	virtual void Deserialize(Buffer& buf) = 0;

	BasePacketType GetBasePacketType() const { return basePacketType; }
	void SetBasePacketType(BasePacketType _type) { basePacketType = _type; }
	size_t GetBufferSize() const { return buffer.Length(); }
protected:
#pragma region Serialize
	//Type -> Byte, (Ÿ��)
	template<typename T_Type,
		std::enable_if_t<std::is_enum_v<T_Type> >* = nullptr >
		inline void PacketTypeSerial(T_Type _type) {
		buffer << _type;
	}

	//String -> Byte, (������)
	inline void StringSerial(std::string _data) {
		buffer << _data;
	}

	//Value -> Byte, (��)
	inline void TypeSerial(char _val) {
		buffer << _val;
	}

	inline void TypeSerial(bool _val) {
		buffer << _val;
	}

	inline void TypeSerial(int16 _val) {
		buffer << _val;
	}

	inline void TypeSerial(int32 _val) {
		buffer << _val;
	}

	inline void TypeSerial(int64 _val) {
		buffer << _val;
	}
#pragma endregion
#pragma region Deserialize
	void DeserializeBuffer(Buffer& _buf) {
		buffer = _buf;
	}
	//Byte -> String, (����, ������)
	inline void StringDeserial(Buffer& _buf, std::string& _data) {
		_buf >> _data;
	}

	//Byte -> Value, (����, ��)
	template<typename T_Arg>
	inline void TypeDeserial(Buffer& _buf, T_Arg& _val) {
		_buf >> _val;
	}
#pragma endregion

protected:
	Buffer buffer;
private:
	BasePacketType basePacketType = BasePacketType::basePacketTypeNone;
};

inline char PacketTypeDeserial(Buffer& _buf) {
	char type;
	_buf >> type;
	return type;
}

#define MAKE_PACKET \
		virtual Buffer& Serialize() override;\
		virtual void Deserialize() override;

#endif